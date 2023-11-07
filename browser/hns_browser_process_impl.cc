/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/browser/hns_browser_process_impl.h"

#include <memory>
#include <string>
#include <utility>

#include "base/functional/bind.h"
#include "base/path_service.h"
#include "base/task/thread_pool.h"
#include "hns/browser/hns_ads/hns_stats_helper.h"
#include "hns/browser/hns_referrals/referrals_service_delegate.h"
#include "hns/browser/hns_shields/ad_block_subscription_download_manager_getter.h"
#include "hns/browser/hns_stats/hns_stats_updater.h"
#include "hns/browser/component_updater/hns_component_updater_configurator.h"
#include "hns/browser/misc_metrics/process_misc_metrics.h"
#include "hns/browser/net/hns_system_request_handler.h"
#include "hns/browser/profiles/hns_profile_manager.h"
#include "hns/browser/themes/hns_dark_mode_utils.h"
#include "hns/common/hns_channel_info.h"
#include "hns/components/hns_ads/browser/component_updater/resource_component.h"
#include "hns/components/hns_component_updater/browser/hns_component_updater_delegate.h"
#include "hns/components/hns_component_updater/browser/hns_on_demand_updater.h"
#include "hns/components/hns_component_updater/browser/local_data_files_service.h"
#include "hns/components/hns_referrals/browser/hns_referrals_service.h"
#include "hns/components/hns_shields/browser/ad_block_regional_service_manager.h"
#include "hns/components/hns_shields/browser/ad_block_service.h"
#include "hns/components/hns_shields/browser/ad_block_subscription_service_manager.h"
#include "hns/components/hns_shields/browser/hns_farbling_service.h"
#include "hns/components/hns_shields/browser/https_everywhere_service.h"
#include "hns/components/hns_shields/common/features.h"
#include "hns/components/hns_sync/network_time_helper.h"
#include "hns/components/constants/pref_names.h"
#include "hns/components/debounce/browser/debounce_component_installer.h"
#include "hns/components/debounce/common/features.h"
#include "hns/components/https_upgrade_exceptions/browser/https_upgrade_exceptions_service.h"
#include "hns/components/localhost_permission/localhost_permission_component.h"
#include "hns/components/ntp_background_images/browser/ntp_background_images_service.h"
#include "hns/components/p3a/buildflags.h"
#include "hns/components/p3a/histograms_hnsizer.h"
#include "hns/components/p3a/p3a_config.h"
#include "hns/components/p3a/p3a_service.h"
#include "hns/services/network/public/cpp/system_request_handler.h"
#include "build/build_config.h"
#include "chrome/browser/component_updater/component_updater_utils.h"
#include "chrome/browser/net/system_network_context_manager.h"
#include "chrome/browser/obsolete_system/obsolete_system.h"
#include "chrome/common/buildflags.h"
#include "chrome/common/channel_info.h"
#include "chrome/common/chrome_paths.h"
#include "chrome/common/pref_names.h"
#include "components/component_updater/component_updater_service.h"
#include "components/component_updater/timer_update_scheduler.h"
#include "content/public/browser/browser_thread.h"
#include "content/public/browser/child_process_security_policy.h"
#include "net/base/features.h"
#include "services/network/public/cpp/resource_request.h"
#include "services/network/public/cpp/shared_url_loader_factory.h"
#include "url/gurl.h"

#if BUILDFLAG(ENABLE_GREASELION)
#include "hns/components/greaselion/browser/greaselion_download_service.h"
#endif

#if BUILDFLAG(ENABLE_TOR)
#include "hns/components/tor/hns_tor_client_updater.h"
#include "hns/components/tor/hns_tor_pluggable_transport_updater.h"
#include "hns/components/tor/pref_names.h"
#endif

#if BUILDFLAG(ENABLE_IPFS)
#include "hns/components/ipfs/hns_ipfs_client_updater.h"
#include "hns/components/ipfs/ipfs_constants.h"
#endif

#if BUILDFLAG(ENABLE_SPEEDREADER)
#include "hns/components/speedreader/speedreader_rewriter_service.h"
#endif

#if BUILDFLAG(IS_ANDROID)
#include "chrome/browser/flags/android/chrome_feature_list.h"
#else
#include "hns/browser/ui/hns_browser_command_controller.h"
#include "chrome/browser/ui/browser.h"
#include "chrome/browser/ui/browser_list.h"
#endif

#if BUILDFLAG(ENABLE_REQUEST_OTR)
#include "hns/components/request_otr/browser/request_otr_component_installer.h"
#include "hns/components/request_otr/common/features.h"
#endif

#if BUILDFLAG(ENABLE_HNS_VPN)
#include "hns/components/hns_vpn/browser/connection/hns_vpn_os_connection_api.h"
#endif

using hns_component_updater::HnsComponent;
using ntp_background_images::NTPBackgroundImagesService;

namespace {

// Initializes callback for SystemRequestHandler
void InitSystemRequestHandlerCallback() {
  network::SystemRequestHandler::OnBeforeSystemRequestCallback
      before_system_request_callback =
          base::BindRepeating(hns::OnBeforeSystemRequest);
  network::SystemRequestHandler::GetInstance()
      ->RegisterOnBeforeSystemRequestCallback(before_system_request_callback);
}

}  // namespace

using content::BrowserThread;

HnsBrowserProcessImpl::~HnsBrowserProcessImpl() = default;

HnsBrowserProcessImpl::HnsBrowserProcessImpl(StartupData* startup_data)
    : BrowserProcessImpl(startup_data) {
  g_browser_process = this;
  g_hns_browser_process = this;

  // early initialize referrals
  hns_referrals_service();

  // Disabled on mobile platforms, see for instance issues/6176
  // Create P3A Service early to catch more histograms. The full initialization
  // should be started once browser process impl is ready.
  p3a_service();
#if BUILDFLAG(HNS_P3A_ENABLED)
  histogram_hnsizer_ = p3a::HistogramsHnsizer::Create();
#endif  // BUILDFLAG(HNS_P3A_ENABLED)

  // initialize ads stats helper
  ads_hns_stats_helper();

  // early initialize hns stats
  hns_stats_updater();

  // early initialize misc metrics
  process_misc_metrics();
}

void HnsBrowserProcessImpl::Init() {
  BrowserProcessImpl::Init();
#if BUILDFLAG(ENABLE_IPFS)
  content::ChildProcessSecurityPolicy::GetInstance()->RegisterWebSafeScheme(
      ipfs::kIPFSScheme);
  content::ChildProcessSecurityPolicy::GetInstance()->RegisterWebSafeScheme(
      ipfs::kIPNSScheme);
#endif
  hns_component_updater::HnsOnDemandUpdater::GetInstance()
      ->RegisterOnDemandUpdateCallback(
          base::BindRepeating(&component_updater::HnsOnDemandUpdate));
  UpdateHnsDarkMode();
  pref_change_registrar_.Add(
      kHnsDarkMode,
      base::BindRepeating(&HnsBrowserProcessImpl::OnHnsDarkModeChanged,
                          base::Unretained(this)));

#if BUILDFLAG(ENABLE_TOR)
  pref_change_registrar_.Add(
      tor::prefs::kTorDisabled,
      base::BindRepeating(&HnsBrowserProcessImpl::OnTorEnabledChanged,
                          base::Unretained(this)));
#endif

  InitSystemRequestHandlerCallback();

#if !BUILDFLAG(IS_ANDROID)
  if (!ObsoleteSystem::IsObsoleteNowOrSoon()) {
    // Clear to show unsupported warning infobar again even if user
    // suppressed it from previous os.
    local_state()->ClearPref(prefs::kSuppressUnsupportedOSWarning);
  }
#endif
}

#if !BUILDFLAG(IS_ANDROID)
void HnsBrowserProcessImpl::StartTearDown() {
  hns_stats_updater_.reset();
  hns_referrals_service_.reset();
  BrowserProcessImpl::StartTearDown();
}

void HnsBrowserProcessImpl::PostDestroyThreads() {
  BrowserProcessImpl::PostDestroyThreads();
  // AdBlockService should outlive its own worker thread.
  ad_block_service_.reset();
}
#endif

hns_component_updater::HnsComponent::Delegate*
HnsBrowserProcessImpl::hns_component_updater_delegate() {
  if (!hns_component_updater_delegate_) {
    hns_component_updater_delegate_ =
        std::make_unique<hns::HnsComponentUpdaterDelegate>(
            component_updater(), local_state(), GetApplicationLocale());
  }
  return hns_component_updater_delegate_.get();
}

ProfileManager* HnsBrowserProcessImpl::profile_manager() {
  DCHECK_CALLED_ON_VALID_SEQUENCE(sequence_checker_);
  if (!created_profile_manager_) {
    CreateProfileManager();
  }
  return profile_manager_.get();
}

void HnsBrowserProcessImpl::StartHnsServices() {
  DCHECK_CURRENTLY_ON(content::BrowserThread::UI);

  https_everywhere_service()->Start();
  resource_component();

  if (base::FeatureList::IsEnabled(net::features::kHnsHttpsByDefault)) {
    https_upgrade_exceptions_service();
  }

  if (base::FeatureList::IsEnabled(
          hns_shields::features::kHnsLocalhostAccessPermission)) {
    localhost_permission_component();
  }

#if BUILDFLAG(ENABLE_GREASELION)
  greaselion_download_service();
#endif
  debounce_component_installer();
#if BUILDFLAG(ENABLE_REQUEST_OTR)
  request_otr_component_installer();
#endif
#if BUILDFLAG(ENABLE_SPEEDREADER)
  speedreader_rewriter_service();
#endif
  URLSanitizerComponentInstaller();
  // Now start the local data files service, which calls all observers.
  local_data_files_service()->Start();

  hns_sync::NetworkTimeHelper::GetInstance()->SetNetworkTimeTracker(
      g_browser_process->network_time_tracker());
}

hns_shields::AdBlockService* HnsBrowserProcessImpl::ad_block_service() {
  if (!ad_block_service_) {
    scoped_refptr<base::SequencedTaskRunner> task_runner(
        base::ThreadPool::CreateSequencedTaskRunner(
            {base::MayBlock(), base::TaskPriority::USER_BLOCKING,
             base::TaskShutdownBehavior::SKIP_ON_SHUTDOWN}));
    ad_block_service_ = std::make_unique<hns_shields::AdBlockService>(
        local_state(), GetApplicationLocale(), component_updater(), task_runner,
        AdBlockSubscriptionDownloadManagerGetter(),
        profile_manager()->user_data_dir().Append(
            profile_manager()->GetInitialProfileDir()));
  }
  return ad_block_service_.get();
}

NTPBackgroundImagesService*
HnsBrowserProcessImpl::ntp_background_images_service() {
  if (!ntp_background_images_service_) {
    ntp_background_images_service_ =
        std::make_unique<NTPBackgroundImagesService>(component_updater(),
                                                     local_state());
    ntp_background_images_service_->Init();
  }

  return ntp_background_images_service_.get();
}

https_upgrade_exceptions::HttpsUpgradeExceptionsService*
HnsBrowserProcessImpl::https_upgrade_exceptions_service() {
  if (!https_upgrade_exceptions_service_) {
    https_upgrade_exceptions_service_ =
        https_upgrade_exceptions::HttpsUpgradeExceptionsServiceFactory(
            local_data_files_service());
  }
  return https_upgrade_exceptions_service_.get();
}

localhost_permission::LocalhostPermissionComponent*
HnsBrowserProcessImpl::localhost_permission_component() {
  if (!base::FeatureList::IsEnabled(
          hns_shields::features::kHnsLocalhostAccessPermission)) {
    return nullptr;
  }

  if (!localhost_permission_component_) {
    localhost_permission_component_ =
        std::make_unique<localhost_permission::LocalhostPermissionComponent>(
            local_data_files_service());
  }
  return localhost_permission_component_.get();
}

#if BUILDFLAG(ENABLE_GREASELION)
greaselion::GreaselionDownloadService*
HnsBrowserProcessImpl::greaselion_download_service() {
  if (!greaselion_download_service_) {
    greaselion_download_service_ = greaselion::GreaselionDownloadServiceFactory(
        local_data_files_service());
  }
  return greaselion_download_service_.get();
}
#endif

debounce::DebounceComponentInstaller*
HnsBrowserProcessImpl::debounce_component_installer() {
  if (!base::FeatureList::IsEnabled(debounce::features::kHnsDebounce)) {
    return nullptr;
  }
  if (!debounce_component_installer_) {
    debounce_component_installer_ =
        std::make_unique<debounce::DebounceComponentInstaller>(
            local_data_files_service());
  }
  return debounce_component_installer_.get();
}

#if BUILDFLAG(ENABLE_REQUEST_OTR)
request_otr::RequestOTRComponentInstallerPolicy*
HnsBrowserProcessImpl::request_otr_component_installer() {
  if (!base::FeatureList::IsEnabled(
          request_otr::features::kHnsRequestOTRTab)) {
    return nullptr;
  }
  if (!request_otr_component_installer_) {
    request_otr_component_installer_ =
        std::make_unique<request_otr::RequestOTRComponentInstallerPolicy>(
            local_data_files_service());
  }
  return request_otr_component_installer_.get();
}
#endif

hns::URLSanitizerComponentInstaller*
HnsBrowserProcessImpl::URLSanitizerComponentInstaller() {
  if (!url_sanitizer_component_installer_) {
    url_sanitizer_component_installer_ =
        std::make_unique<hns::URLSanitizerComponentInstaller>(
            local_data_files_service());
  }
  return url_sanitizer_component_installer_.get();
}

hns_shields::HTTPSEverywhereService*
HnsBrowserProcessImpl::https_everywhere_service() {
  if (!created_https_everywhere_service_) {
    https_everywhere_service_ = hns_shields::HTTPSEverywhereServiceFactory(
        hns_component_updater_delegate()->GetTaskRunner());
    created_https_everywhere_service_ = true;
  }
  return https_everywhere_service_.get();
}

hns_component_updater::LocalDataFilesService*
HnsBrowserProcessImpl::local_data_files_service() {
  if (!local_data_files_service_) {
    local_data_files_service_ =
        hns_component_updater::LocalDataFilesServiceFactory(
            hns_component_updater_delegate());
  }
  return local_data_files_service_.get();
}

void HnsBrowserProcessImpl::UpdateHnsDarkMode() {
  // Update with proper system theme to make hns theme and base ui components
  // theme use same theme.
  dark_mode::SetSystemDarkMode(dark_mode::GetHnsDarkModeType());
}

void HnsBrowserProcessImpl::OnHnsDarkModeChanged() {
  UpdateHnsDarkMode();
}

#if BUILDFLAG(ENABLE_TOR)
tor::HnsTorClientUpdater* HnsBrowserProcessImpl::tor_client_updater() {
  if (tor_client_updater_) {
    return tor_client_updater_.get();
  }

  base::FilePath user_data_dir;
  base::PathService::Get(chrome::DIR_USER_DATA, &user_data_dir);

  tor_client_updater_ = std::make_unique<tor::HnsTorClientUpdater>(
      hns_component_updater_delegate(), local_state(), user_data_dir);
  return tor_client_updater_.get();
}

tor::HnsTorPluggableTransportUpdater*
HnsBrowserProcessImpl::tor_pluggable_transport_updater() {
  if (!tor_pluggable_transport_updater_) {
    base::FilePath user_data_dir;
    base::PathService::Get(chrome::DIR_USER_DATA, &user_data_dir);

    tor_pluggable_transport_updater_ =
        std::make_unique<tor::HnsTorPluggableTransportUpdater>(
            hns_component_updater_delegate(), local_state(), user_data_dir);
  }
  return tor_pluggable_transport_updater_.get();
}

void HnsBrowserProcessImpl::OnTorEnabledChanged() {
  // Update all browsers' tor command status.
  for (Browser* browser : *BrowserList::GetInstance()) {
    static_cast<chrome::HnsBrowserCommandController*>(
        browser->command_controller())
        ->UpdateCommandForTor();
  }
}
#endif

p3a::P3AService* HnsBrowserProcessImpl::p3a_service() {
#if BUILDFLAG(HNS_P3A_ENABLED)
  if (p3a_service_) {
    return p3a_service_.get();
  }
  p3a_service_ = base::MakeRefCounted<p3a::P3AService>(
      *local_state(), hns::GetChannelName(),
      local_state()->GetString(kWeekOfInstallation),
      p3a::P3AConfig::LoadFromCommandLine());
  p3a_service()->InitCallbacks();
  return p3a_service_.get();
#else
  return nullptr;
#endif  // BUILDFLAG(HNS_P3A_ENABLED)
}

hns::HnsReferralsService*
HnsBrowserProcessImpl::hns_referrals_service() {
  if (!hns_referrals_service_) {
    hns_referrals_service_ = std::make_unique<hns::HnsReferralsService>(
        local_state(), hns_stats::GetAPIKey(),
        hns_stats::GetPlatformIdentifier());
    hns_referrals_service_->set_delegate(
        std::make_unique<ReferralsServiceDelegate>(
            hns_referrals_service_.get()));
  }
  return hns_referrals_service_.get();
}

hns_stats::HnsStatsUpdater* HnsBrowserProcessImpl::hns_stats_updater() {
  if (!hns_stats_updater_) {
    hns_stats_updater_ = std::make_unique<hns_stats::HnsStatsUpdater>(
        local_state(), g_browser_process->profile_manager());
  }
  return hns_stats_updater_.get();
}

hns_ads::HnsStatsHelper* HnsBrowserProcessImpl::ads_hns_stats_helper() {
  if (!hns_stats_helper_) {
    hns_stats_helper_ = std::make_unique<hns_ads::HnsStatsHelper>();
  }
  return hns_stats_helper_.get();
}

hns_ads::ResourceComponent* HnsBrowserProcessImpl::resource_component() {
  if (!resource_component_) {
    resource_component_ = std::make_unique<hns_ads::ResourceComponent>(
        hns_component_updater_delegate());
  }
  return resource_component_.get();
}

void HnsBrowserProcessImpl::CreateProfileManager() {
  DCHECK(!created_profile_manager_ && !profile_manager_);
  created_profile_manager_ = true;

  base::FilePath user_data_dir;
  base::PathService::Get(chrome::DIR_USER_DATA, &user_data_dir);
  profile_manager_ = std::make_unique<HnsProfileManager>(user_data_dir);
}

NotificationPlatformBridge*
HnsBrowserProcessImpl::notification_platform_bridge() {
  return BrowserProcessImpl::notification_platform_bridge();
}

#if BUILDFLAG(ENABLE_SPEEDREADER)
speedreader::SpeedreaderRewriterService*
HnsBrowserProcessImpl::speedreader_rewriter_service() {
  if (!speedreader_rewriter_service_) {
    speedreader_rewriter_service_ =
        std::make_unique<speedreader::SpeedreaderRewriterService>();
  }
  return speedreader_rewriter_service_.get();
}
#endif  // BUILDFLAG(ENABLE_SPEEDREADER)

#if BUILDFLAG(ENABLE_IPFS)
ipfs::HnsIpfsClientUpdater* HnsBrowserProcessImpl::ipfs_client_updater() {
  if (ipfs_client_updater_) {
    return ipfs_client_updater_.get();
  }

  base::FilePath user_data_dir;
  base::PathService::Get(chrome::DIR_USER_DATA, &user_data_dir);

  ipfs_client_updater_ = ipfs::HnsIpfsClientUpdaterFactory(
      hns_component_updater_delegate(), user_data_dir);
  return ipfs_client_updater_.get();
}
#endif  // BUILDFLAG(ENABLE_IPFS)

#if BUILDFLAG(ENABLE_HNS_VPN)
hns_vpn::HnsVPNOSConnectionAPI*
HnsBrowserProcessImpl::hns_vpn_os_connection_api() {
  if (hns_vpn_os_connection_api_) {
    return hns_vpn_os_connection_api_.get();
  }

  hns_vpn_os_connection_api_ = hns_vpn::CreateHnsVPNConnectionAPI(
      shared_url_loader_factory(), local_state(), chrome::GetChannel());
  return hns_vpn_os_connection_api_.get();
}
#endif

hns::HnsFarblingService* HnsBrowserProcessImpl::hns_farbling_service() {
  if (!hns_farbling_service_) {
    hns_farbling_service_ = std::make_unique<hns::HnsFarblingService>();
  }
  return hns_farbling_service_.get();
}

misc_metrics::ProcessMiscMetrics*
HnsBrowserProcessImpl::process_misc_metrics() {
  if (!process_misc_metrics_) {
    process_misc_metrics_ =
        std::make_unique<misc_metrics::ProcessMiscMetrics>(local_state());
  }
  return process_misc_metrics_.get();
}
