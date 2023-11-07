/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/browser/hns_local_state_prefs.h"

#include <string>

#include "base/values.h"
#include "hns/browser/hns_ads/hns_stats_helper.h"
#include "hns/browser/hns_stats/hns_stats_updater.h"
#include "hns/browser/metrics/buildflags/buildflags.h"
#include "hns/browser/metrics/metrics_reporting_util.h"
#include "hns/browser/misc_metrics/process_misc_metrics.h"
#include "hns/browser/ntp_background/ntp_p3a_helper_impl.h"
#include "hns/browser/playlist/playlist_service_factory.h"
#include "hns/browser/themes/hns_dark_mode_utils.h"
#include "hns/components/hns_ads/browser/ads_service.h"
#include "hns/components/hns_referrals/browser/hns_referrals_service.h"
#include "hns/components/hns_search_conversion/p3a.h"
#include "hns/components/hns_shields/browser/ad_block_service.h"
#include "hns/components/hns_shields/browser/hns_shields_p3a.h"
#include "hns/components/hns_vpn/common/buildflags/buildflags.h"
#include "hns/components/hns_wallet/browser/hns_wallet_prefs.h"
#include "hns/components/constants/pref_names.h"
#include "hns/components/decentralized_dns/core/utils.h"
#include "hns/components/misc_metrics/general_browser_usage.h"
#include "hns/components/misc_metrics/page_metrics_service.h"
#include "hns/components/misc_metrics/privacy_hub_metrics.h"
#include "hns/components/ntp_background_images/browser/ntp_background_images_service.h"
#include "hns/components/ntp_background_images/browser/view_counter_service.h"
#include "hns/components/p3a/p3a_service.h"
#include "hns/components/skus/browser/skus_utils.h"
#include "hns/components/tor/buildflags/buildflags.h"
#include "build/build_config.h"
#include "chrome/common/pref_names.h"
#include "components/metrics/metrics_pref_names.h"
#include "components/prefs/pref_registry_simple.h"
#include "third_party/widevine/cdm/buildflags.h"

#if BUILDFLAG(ENABLE_TOR)
#include "hns/components/tor/tor_profile_service.h"
#endif

#include "hns/browser/ui/webui/new_tab_page/hns_new_tab_message_handler.h"

#if !BUILDFLAG(IS_ANDROID)
#include "hns/browser/p3a/p3a_core_metrics.h"
#include "hns/browser/ui/whats_new/whats_new_util.h"
#include "chrome/browser/first_run/first_run.h"
#endif  // !BUILDFLAG(IS_ANDROID)

#if BUILDFLAG(ENABLE_HNS_VPN)
#include "hns/components/hns_vpn/common/hns_vpn_utils.h"
#endif

#if BUILDFLAG(ENABLE_WIDEVINE)
#include "hns/browser/widevine/widevine_utils.h"
#endif

namespace hns {

void RegisterLocalStatePrefsForMigration(PrefRegistrySimple* registry) {
#if BUILDFLAG(ENABLE_WIDEVINE)
  RegisterWidevineLocalstatePrefsForMigration(registry);
#endif

#if !BUILDFLAG(IS_ANDROID)
  // Added 10/2022
  registry->RegisterBooleanPref(kDefaultBrowserPromptEnabled, true);
  HnsUptimeTracker::RegisterPrefsForMigration(registry);
#endif

  hns_wallet::RegisterLocalStatePrefsForMigration(registry);
  hns_search_conversion::p3a::RegisterLocalStatePrefsForMigration(registry);
}

void RegisterLocalStatePrefs(PrefRegistrySimple* registry) {
  hns_shields::RegisterPrefsForAdBlockService(registry);
  hns_stats::RegisterLocalStatePrefs(registry);
  ntp_background_images::NTPBackgroundImagesService::RegisterLocalStatePrefs(
      registry);
  ntp_background_images::ViewCounterService::RegisterLocalStatePrefs(registry);
  RegisterPrefsForHnsReferralsService(registry);
#if BUILDFLAG(IS_MAC)
  // Turn off super annoying 'Hold to quit'
  registry->SetDefaultPrefValue(prefs::kConfirmToQuitEnabled,
                                base::Value(false));
#endif
#if BUILDFLAG(ENABLE_TOR)
  tor::TorProfileService::RegisterLocalStatePrefs(registry);
#endif
  registry->SetDefaultPrefValue(
      metrics::prefs::kMetricsReportingEnabled,
      base::Value(GetDefaultPrefValueForMetricsReporting()));

  p3a::P3AService::RegisterPrefs(registry,
#if !BUILDFLAG(IS_ANDROID)
                                 first_run::IsChromeFirstRun());
#else
                                 // HnsP3AService::RegisterPrefs
                                 // doesn't use this arg on Android
                                 false);
#endif  // !BUILDFLAG(IS_ANDROID)

  hns_shields::RegisterShieldsP3ALocalPrefs(registry);
#if !BUILDFLAG(IS_ANDROID)
  HnsNewTabMessageHandler::RegisterLocalStatePrefs(registry);
  HnsWindowTracker::RegisterPrefs(registry);
  HnsUptimeTracker::RegisterPrefs(registry);
  dark_mode::RegisterHnsDarkModeLocalStatePrefs(registry);
  whats_new::RegisterLocalStatePrefs(registry);
#endif

#if BUILDFLAG(ENABLE_CRASH_DIALOG)
  registry->RegisterBooleanPref(kDontAskForCrashReporting, false);
#endif

#if BUILDFLAG(ENABLE_WIDEVINE)
  RegisterWidevineLocalstatePrefs(registry);
#endif

  decentralized_dns::RegisterLocalStatePrefs(registry);

  RegisterLocalStatePrefsForMigration(registry);

  hns_search_conversion::p3a::RegisterLocalStatePrefs(registry);

#if BUILDFLAG(ENABLE_HNS_VPN)
  hns_vpn::RegisterLocalStatePrefs(registry);
  skus::RegisterLocalStatePrefs(registry);
#endif

  registry->RegisterStringPref(::prefs::kHnsVpnDnsConfig, std::string());

  ntp_background_images::NTPP3AHelperImpl::RegisterLocalStatePrefs(registry);

  hns_wallet::RegisterLocalStatePrefs(registry);

  misc_metrics::ProcessMiscMetrics::RegisterPrefs(registry);
  misc_metrics::PageMetricsService::RegisterPrefs(registry);
  hns_ads::HnsStatsHelper::RegisterLocalStatePrefs(registry);
  misc_metrics::GeneralBrowserUsage::RegisterPrefs(registry);

  playlist::PlaylistServiceFactory::RegisterLocalStatePrefs(registry);
}

}  // namespace hns
