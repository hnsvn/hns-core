/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/browser/hns_stats/hns_stats_updater.h"

#include <memory>
#include <utility>

#include "base/barrier_closure.h"
#include "base/command_line.h"
#include "base/metrics/histogram_macros.h"
#include "base/system/sys_info.h"
#include "hns/browser/hns_stats/hns_stats_updater_params.h"
#include "hns/browser/hns_stats/buildflags.h"
#include "hns/browser/hns_stats/switches.h"
#include "hns/common/hns_channel_info.h"
#include "hns/components/hns_ads/core/public/prefs/pref_names.h"
#include "hns/components/hns_referrals/common/pref_names.h"
#include "hns/components/hns_stats/browser/hns_stats_updater_util.h"
#include "hns/components/hns_wallet/browser/pref_names.h"
#include "hns/components/constants/network_constants.h"
#include "hns/components/constants/pref_names.h"
#include "hns/components/misc_metrics/general_browser_usage.h"
#include "hns/components/rpill/common/rpill.h"
#include "hns/components/version_info/version_info.h"
#include "chrome/browser/browser_process.h"
#include "chrome/browser/net/system_network_context_manager.h"
#include "chrome/browser/profiles/profile_manager.h"
#include "chrome/common/channel_info.h"
#include "components/prefs/pref_change_registrar.h"
#include "components/prefs/pref_registry_simple.h"
#include "components/prefs/pref_service.h"
#include "content/public/browser/browser_thread.h"
#include "net/base/load_flags.h"
#include "net/base/url_util.h"
#include "net/http/http_response_headers.h"
#include "net/traffic_annotation/network_traffic_annotation.h"
#include "services/network/public/cpp/resource_request.h"
#include "services/network/public/cpp/simple_url_loader.h"
#include "services/network/public/mojom/fetch_api.mojom-shared.h"

namespace hns_stats {

const char kP3AMonthlyPingHistogramName[] = "Hns.Core.UsageMonthly";
const char kP3ADailyPingHistogramName[] = "Hns.Core.UsageDaily";

namespace {

constexpr char kInvalidUrl[] = "https://no-thanks.invalid";

HnsStatsUpdater::StatsUpdatedCallback* g_testing_stats_updated_callback =
    nullptr;
HnsStatsUpdater::StatsUpdatedCallback* g_testing_stats_threshold_callback =
    nullptr;

// Ping the update server shortly after startup.
static constexpr int kUpdateServerStartupPingDelaySeconds = 3;

// Every five minutes, check if we need to ping the update server for
// today.
static constexpr int kUpdateServerPeriodicPingFrequencySeconds = 5 * 60;

static constexpr int kMinimumUsageThreshold = 3;

GURL GetUpdateURL(
    const GURL& base_update_url,
    const hns_stats::HnsStatsUpdaterParams& stats_updater_params) {
  return stats_updater_params.GetUpdateURL(
      base_update_url, hns_stats::GetPlatformIdentifier(),
      hns::GetChannelName(),
      version_info::GetHnsVersionWithoutChromiumMajorVersion());
}

net::NetworkTrafficAnnotationTag AnonymousStatsAnnotation() {
  return net::DefineNetworkTrafficAnnotation("hns_stats_updater", R"(
    semantics {
      sender:
        "Hns Stats Updater"
      description:
        "This service sends anonymous usage statistics to Hns."
      trigger:
        "Stats are automatically sent at intervals while Hns "
        "is running."
      data: "Anonymous usage statistics."
      destination: WEBSITE
    }
    policy {
      cookies_allowed: NO
      setting:
        "This feature cannot be disabled by settings."
      policy_exception_justification:
        "Not implemented."
    })");
}

}  // anonymous namespace

HnsStatsUpdater::HnsStatsUpdater(PrefService* pref_service,
                                     ProfileManager* profile_manager)
    : pref_service_(pref_service),
      profile_manager_(profile_manager),
      testing_url_loader_factory_(nullptr) {
  const base::CommandLine& command_line =
      *base::CommandLine::ForCurrentProcess();
  if (command_line.HasSwitch(switches::kHnsStatsUpdaterServer)) {
    usage_server_ =
        command_line.GetSwitchValueASCII(switches::kHnsStatsUpdaterServer);
    if (!GURL(usage_server_).is_valid())
      LOG(ERROR) << "bad url given as stats updater url: " << usage_server_;
  } else {
    usage_server_ = BUILDFLAG(HNS_USAGE_SERVER);
  }

  general_browser_usage_p3a_ =
      std::make_unique<misc_metrics::GeneralBrowserUsage>(pref_service);

  if (profile_manager != nullptr) {
    g_browser_process->profile_manager()->AddObserver(this);
  }

  Start();
}

HnsStatsUpdater::~HnsStatsUpdater() {
  if (profile_manager_ != nullptr) {
    g_browser_process->profile_manager()->RemoveObserver(this);
  }
}

void HnsStatsUpdater::Start() {
  // Startup timer, only initiated once we've checked for a promo
  // code.
  DCHECK(!server_ping_startup_timer_);
  server_ping_startup_timer_ = std::make_unique<base::OneShotTimer>();

  QueueServerPing();

  // Periodic timer.
  DCHECK(!server_ping_periodic_timer_);
  server_ping_periodic_timer_ = std::make_unique<base::RepeatingTimer>();
  server_ping_periodic_timer_->Start(
      FROM_HERE, base::Seconds(kUpdateServerPeriodicPingFrequencySeconds), this,
      &HnsStatsUpdater::OnServerPingTimerFired);

  // Record ping for P3A.
  UMA_HISTOGRAM_BOOLEAN(kP3AMonthlyPingHistogramName, true);
  UMA_HISTOGRAM_BOOLEAN(kP3ADailyPingHistogramName, true);
}

void HnsStatsUpdater::Stop() {
  server_ping_startup_timer_.reset();
  server_ping_periodic_timer_.reset();
}

bool HnsStatsUpdater::MaybeDoThresholdPing(int score) {
  DCHECK_CURRENTLY_ON(content::BrowserThread::UI);
  threshold_score_ += score;

  // We only do this once
  if (HasDoneThresholdPing())
    return true;

  const bool reporting_enabled =
      pref_service_->GetBoolean(kStatsReportingEnabled);
  if (!reporting_enabled) {
    if (g_testing_stats_threshold_callback)
      g_testing_stats_threshold_callback->Run(GURL(kInvalidUrl));
    return false;
  }

  const bool threshold_met = threshold_score_ >= kMinimumUsageThreshold;
  // We don't want to start the threshold ping if:
  //   (1) The standard ping is still waiting to be sent.
  //   (2) Stats is blocked by referral initialization or ads.
  // The standard usage ping will set the url and call us back.
  if (server_ping_startup_timer_->IsRunning() || !stats_startup_complete_)
    return threshold_met;

  if (threshold_met) {
    SendUserTriggeredPing();
    return true;
  }
  return false;
}

network::mojom::URLLoaderFactory* HnsStatsUpdater::GetURLLoaderFactory() {
  if (testing_url_loader_factory_ != nullptr) {
    return testing_url_loader_factory_.get();
  }
  return g_browser_process->system_network_context_manager()
      ->GetURLLoaderFactory();
}

// static
void HnsStatsUpdater::SetStatsUpdatedCallbackForTesting(
    StatsUpdatedCallback* stats_updated_callback) {
  g_testing_stats_updated_callback = stats_updated_callback;
}

// static
void HnsStatsUpdater::SetStatsThresholdCallbackForTesting(
    StatsUpdatedCallback* stats_threshold_callback) {
  g_testing_stats_threshold_callback = stats_threshold_callback;
}

void HnsStatsUpdater::SetURLLoaderFactoryForTesting(
    scoped_refptr<network::SharedURLLoaderFactory> url_loader_factory) {
  testing_url_loader_factory_ = url_loader_factory;
}

void HnsStatsUpdater::SetUsageServerForTesting(
    const std::string& usage_server) {
  usage_server_ = usage_server;
}

GURL HnsStatsUpdater::BuildStatsEndpoint(const std::string& path) {
  return GURL(usage_server_ + path);
}

void HnsStatsUpdater::OnSimpleLoaderComplete(
    std::unique_ptr<hns_stats::HnsStatsUpdaterParams> stats_updater_params,
    scoped_refptr<net::HttpResponseHeaders> headers) {
  DCHECK_CURRENTLY_ON(content::BrowserThread::UI);
  GURL final_url = simple_url_loader_->GetFinalURL();
  int response_code = -1;
  if (headers)
    response_code = headers->response_code();
  if (simple_url_loader_->NetError() != net::OK || response_code < 200 ||
      response_code > 299) {
    VLOG(1) << "Failed to send usage stats to update server"
            << ", error: " << simple_url_loader_->NetError()
            << ", response code: " << response_code
            << ", url: " << final_url.spec();
    return;
  }

  bool first_check_made = pref_service_->GetBoolean(kFirstCheckMade);

  // We need to set this *before* params are saved.
  if (!first_check_made && !HasDoneThresholdPing()) {
    auto endpoint = BuildStatsEndpoint(kHnsUsageThresholdPath);
    auto threshold_query = GetUpdateURL(endpoint, *stats_updater_params);
    // Unfortunately we need to serialize this in case the user starts
    // the browser, stats ping goes, then we lose the original params.
    pref_service_->SetString(kThresholdQuery, threshold_query.spec());
  }

  // The request to the update server succeeded, so it's safe to save
  // the usage preferences now.
  stats_updater_params->SavePrefs();

  // Inform the client that the stats ping completed, if requested.
  if (g_testing_stats_updated_callback)
    g_testing_stats_updated_callback->Run(final_url);

  // In case the first call was blocked by our timer.
  (void)MaybeDoThresholdPing(0);

  // Log the full URL of the stats ping.
  VLOG(1) << "Hns stats ping, url: " << final_url.spec();
}

void HnsStatsUpdater::OnThresholdLoaderComplete(
    scoped_refptr<net::HttpResponseHeaders> headers) {
  DCHECK_CURRENTLY_ON(content::BrowserThread::UI);
  GURL final_url = simple_url_loader_->GetFinalURL();
  int response_code = -1;
  if (headers)
    response_code = headers->response_code();
  if (simple_url_loader_->NetError() != net::OK || response_code < 200 ||
      response_code > 299) {
    VLOG(1) << "Failed to send threshold ping to update server"
            << ", error: " << simple_url_loader_->NetError()
            << ", response code: " << response_code
            << ", url: " << final_url.spec();
    return;
  }

  // Inform the client that the threshold ping completed, if requested.
  if (g_testing_stats_threshold_callback)
    g_testing_stats_threshold_callback->Run(final_url);

  // We only send this query once.
  DisableThresholdPing();

  // Log the full URL of the stats ping.
  VLOG(1) << "Hns stats ping, url: " << final_url.spec();
}

void HnsStatsUpdater::OnServerPingTimerFired() {
  // If we already pinged the stats server today, then we're done.
  std::string today_ymd = hns_stats::GetDateAsYMD(base::Time::Now());
  std::string last_check_ymd = pref_service_->GetString(kLastCheckYMD);
  if (base::CompareCaseInsensitiveASCII(today_ymd, last_check_ymd) == 0)
    return;

  const bool reporting_enabled =
      pref_service_->GetBoolean(kStatsReportingEnabled);
  if (!reporting_enabled) {
    if (g_testing_stats_updated_callback)
      g_testing_stats_updated_callback->Run(GURL(kInvalidUrl));
    return;
  }
  SendServerPing();
}

bool HnsStatsUpdater::IsReferralInitialized() {
  return pref_service_->GetBoolean(kReferralInitialization) ||
         pref_service_->GetBoolean(kReferralCheckedForPromoCodeFile);
}

bool HnsStatsUpdater::IsAdsEnabled() {
  return pref_service_->GetBoolean(hns_ads::prefs::kEnabledForLastProfile);
}

bool HnsStatsUpdater::HasDoneThresholdPing() {
  return pref_service_->GetBoolean(kThresholdCheckMade);
}

void HnsStatsUpdater::DisableThresholdPing() {
  pref_service_->SetBoolean(kThresholdCheckMade, true);
  pref_service_->ClearPref(kThresholdQuery);
}

void HnsStatsUpdater::OnProfileAdded(Profile* profile) {
  general_browser_usage_p3a_->ReportProfileCount(
      g_browser_process->profile_manager()->GetNumberOfProfiles());
}

void HnsStatsUpdater::QueueServerPing() {
  const bool referrals_initialized = IsReferralInitialized();
  const bool ads_enabled = IsAdsEnabled();
  int num_closures = 0;

  // Note: We don't have the callbacks here because otherwise there is a race
  // condition whereby the callback completes before the barrier has been
  // initialized.
  if (!referrals_initialized) {
    ++num_closures;
  }
  if (ads_enabled) {
    ++num_closures;
  }

  // Note: If num_closures == 0, the callback runs immediately
  stats_preconditions_barrier_ = base::BarrierClosure(
      num_closures,
      base::BindOnce(&HnsStatsUpdater::StartServerPingStartupTimer,
                     weak_ptr_factory_.GetWeakPtr()));
  if (!referrals_initialized) {
    pref_change_registrar_ = std::make_unique<PrefChangeRegistrar>();
    pref_change_registrar_->Init(pref_service_);
    pref_change_registrar_->Add(
        kReferralInitialization,
        base::BindRepeating(&HnsStatsUpdater::OnReferralInitialization,
                            base::Unretained(this)));
  }

  if (ads_enabled) {
    DetectUncertainFuture();
  }
}

void HnsStatsUpdater::DetectUncertainFuture() {
  hns_rpill::DetectUncertainFuture(
      base::BindOnce(&HnsStatsUpdater::OnDetectUncertainFuture,
                     weak_ptr_factory_.GetWeakPtr()));
}

void HnsStatsUpdater::OnReferralInitialization() {
  pref_change_registrar_ = nullptr;
  if (stats_preconditions_barrier_) {
    stats_preconditions_barrier_.Run();
  }
}

void HnsStatsUpdater::OnDetectUncertainFuture(
    const bool is_uncertain_future) {
  if (is_uncertain_future) {
    arch_ = ProcessArch::kArchVirt;
  } else {
    arch_ = ProcessArch::kArchMetal;
  }
  if (stats_preconditions_barrier_) {
    stats_preconditions_barrier_.Run();
  }
}

void HnsStatsUpdater::StartServerPingStartupTimer() {
  stats_preconditions_barrier_.Reset();
  stats_startup_complete_ = true;
  server_ping_startup_timer_->Start(
      FROM_HERE, base::Seconds(kUpdateServerStartupPingDelaySeconds), this,
      &HnsStatsUpdater::OnServerPingTimerFired);
}

void HnsStatsUpdater::SendServerPing() {
  DCHECK_CURRENTLY_ON(content::BrowserThread::UI);
  auto traffic_annotation = AnonymousStatsAnnotation();
  auto resource_request = std::make_unique<network::ResourceRequest>();

  auto stats_updater_params =
      std::make_unique<hns_stats::HnsStatsUpdaterParams>(pref_service_,
                                                             arch_);

  auto endpoint = BuildStatsEndpoint(kHnsUsageStandardPath);
  resource_request->url = GetUpdateURL(endpoint, *stats_updater_params);
  resource_request->credentials_mode = network::mojom::CredentialsMode::kOmit;
  resource_request->load_flags = net::LOAD_DO_NOT_SAVE_COOKIES |
                                 net::LOAD_BYPASS_CACHE |
                                 net::LOAD_DISABLE_CACHE;
  resource_request->headers.SetHeader("X-Hns-API-Key",
                                      hns_stats::GetAPIKey());
  network::mojom::URLLoaderFactory* loader_factory = GetURLLoaderFactory();
  simple_url_loader_ = network::SimpleURLLoader::Create(
      std::move(resource_request), traffic_annotation);
  simple_url_loader_->SetRetryOptions(
      1, network::SimpleURLLoader::RETRY_ON_NETWORK_CHANGE);
  simple_url_loader_->DownloadHeadersOnly(
      loader_factory,
      base::BindOnce(&HnsStatsUpdater::OnSimpleLoaderComplete,
                     base::Unretained(this), std::move(stats_updater_params)));
}

void HnsStatsUpdater::SendUserTriggeredPing() {
  DCHECK_CURRENTLY_ON(content::BrowserThread::UI);
  auto traffic_annotation = AnonymousStatsAnnotation();
  auto resource_request = std::make_unique<network::ResourceRequest>();

  // This pref is only set when kFirstCheckMade is false,
  // so if it is empty, we have an existing user. Disable
  // threshold ping and don't send a request.
  auto threshold_query = pref_service_->GetString(kThresholdQuery);
  if (threshold_query.empty()) {
    DisableThresholdPing();
    return;
  }

  resource_request->url = GURL(threshold_query);
  resource_request->credentials_mode = network::mojom::CredentialsMode::kOmit;
  resource_request->load_flags = net::LOAD_DO_NOT_SAVE_COOKIES |
                                 net::LOAD_BYPASS_CACHE |
                                 net::LOAD_DISABLE_CACHE;
  resource_request->headers.SetHeader("X-Hns-API-Key",
                                      hns_stats::GetAPIKey());
  network::mojom::URLLoaderFactory* loader_factory = GetURLLoaderFactory();
  simple_url_loader_ = network::SimpleURLLoader::Create(
      std::move(resource_request), traffic_annotation);
  simple_url_loader_->SetRetryOptions(
      1, network::SimpleURLLoader::RETRY_ON_NETWORK_CHANGE);
  simple_url_loader_->DownloadHeadersOnly(
      loader_factory,
      base::BindOnce(&HnsStatsUpdater::OnThresholdLoaderComplete,
                     base::Unretained(this)));
}

///////////////////////////////////////////////////////////////////////////////

void RegisterLocalStatePrefs(PrefRegistrySimple* registry) {
  registry->RegisterBooleanPref(kFirstCheckMade, false);
  registry->RegisterBooleanPref(kThresholdCheckMade, false);
  registry->RegisterBooleanPref(kStatsReportingEnabled, true);
  registry->RegisterStringPref(kThresholdQuery, std::string());
  registry->RegisterIntegerPref(kLastCheckWOY, 0);
  registry->RegisterIntegerPref(kLastCheckMonth, 0);
  registry->RegisterStringPref(kLastCheckYMD, std::string());
  registry->RegisterStringPref(kWeekOfInstallation, std::string());
  registry->RegisterTimePref(kHnsWalletPingReportedUnlockTime, base::Time());
}

}  // namespace hns_stats
