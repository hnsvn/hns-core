/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/browser/browser_context_keyed_service_factories.h"

#include "base/feature_list.h"
#include "hns/browser/hns_adaptive_captcha/hns_adaptive_captcha_service_factory.h"
#include "hns/browser/hns_ads/ads_service_factory.h"
#include "hns/browser/hns_federated/hns_federated_service_factory.h"
#include "hns/browser/hns_news/hns_news_controller_factory.h"
#include "hns/browser/hns_rewards/rewards_service_factory.h"
#include "hns/browser/hns_shields/ad_block_pref_service_factory.h"
#include "hns/browser/hns_wallet/asset_ratio_service_factory.h"
#include "hns/browser/hns_wallet/hns_wallet_ipfs_service_factory.h"
#include "hns/browser/hns_wallet/hns_wallet_service_factory.h"
#include "hns/browser/hns_wallet/json_rpc_service_factory.h"
#include "hns/browser/hns_wallet/keyring_service_factory.h"
#include "hns/browser/hns_wallet/notifications/wallet_notification_service_factory.h"
#include "hns/browser/hns_wallet/simulation_service_factory.h"
#include "hns/browser/hns_wallet/swap_service_factory.h"
#include "hns/browser/hns_wallet/tx_service_factory.h"
#include "hns/browser/debounce/debounce_service_factory.h"
#include "hns/browser/ephemeral_storage/ephemeral_storage_service_factory.h"
#include "hns/browser/ethereum_remote_client/buildflags/buildflags.h"
#include "hns/browser/misc_metrics/page_metrics_service_factory.h"
#include "hns/browser/ntp_background/view_counter_service_factory.h"
#include "hns/browser/permissions/permission_lifetime_manager_factory.h"
#include "hns/browser/profiles/hns_renderer_updater_factory.h"
#include "hns/browser/search_engines/search_engine_provider_service_factory.h"
#include "hns/browser/search_engines/search_engine_tracker.h"
#include "hns/browser/sync/hns_sync_alerts_service_factory.h"
#include "hns/browser/url_sanitizer/url_sanitizer_service_factory.h"
#include "hns/components/hns_perf_predictor/browser/named_third_party_registry_factory.h"
#include "hns/components/hns_vpn/common/buildflags/buildflags.h"
#include "hns/components/commander/common/buildflags/buildflags.h"
#include "hns/components/greaselion/browser/buildflags/buildflags.h"
#include "hns/components/ipfs/buildflags/buildflags.h"
#include "hns/components/playlist/common/buildflags/buildflags.h"
#include "hns/components/request_otr/common/buildflags/buildflags.h"
#include "hns/components/speedreader/common/buildflags/buildflags.h"
#include "hns/components/tor/buildflags/buildflags.h"

#if BUILDFLAG(ENABLE_HNS_VPN)
#include "hns/browser/hns_vpn/hns_vpn_service_factory.h"
#include "hns/browser/skus/skus_service_factory.h"
#endif

#if BUILDFLAG(ENABLE_GREASELION)
#include "hns/browser/greaselion/greaselion_service_factory.h"
#endif

#if !BUILDFLAG(IS_ANDROID)
#include "hns/browser/ui/bookmark/bookmark_prefs_service_factory.h"
#include "hns/browser/ui/commands/accelerator_service_factory.h"
#include "hns/browser/ui/tabs/features.h"
#include "hns/browser/ui/tabs/shared_pinned_tab_service_factory.h"
#include "hns/components/commands/common/features.h"
#else
#include "hns/browser/hns_shields/cookie_list_opt_in_service_factory.h"
#include "hns/browser/hns_shields/filter_list_service_factory.h"
#include "hns/browser/ntp_background/android/ntp_background_images_bridge.h"
#endif

#if BUILDFLAG(ETHEREUM_REMOTE_CLIENT_ENABLED)
#include "hns/browser/ethereum_remote_client/ethereum_remote_client_service_factory.h"
#endif

#if BUILDFLAG(ENABLE_IPFS)
#include "hns/browser/ipfs/ipfs_service_factory.h"
#endif

#if BUILDFLAG(ENABLE_IPFS_LOCAL_NODE)
#include "hns/browser/hns_wallet/hns_wallet_auto_pin_service_factory.h"
#include "hns/browser/hns_wallet/hns_wallet_pin_service_factory.h"
#endif

#if BUILDFLAG(ENABLE_TOR)
#include "hns/browser/tor/tor_profile_service_factory.h"
#endif

#if BUILDFLAG(ENABLE_PLAYLIST)
#include "hns/browser/playlist/playlist_service_factory.h"
#include "hns/components/playlist/common/features.h"
#endif

#if BUILDFLAG(ENABLE_COMMANDER)
#include "hns/browser/ui/commander/commander_service_factory.h"
#include "hns/components/commander/common/features.h"
#endif

#if defined(TOOLKIT_VIEWS)
#include "hns/browser/ui/sidebar/sidebar_service_factory.h"
#endif

#if BUILDFLAG(ENABLE_SPEEDREADER)
#include "hns/browser/speedreader/speedreader_service_factory.h"
#endif

#if BUILDFLAG(ENABLE_REQUEST_OTR)
#include "hns/browser/request_otr/request_otr_service_factory.h"
#endif

namespace hns {

void EnsureBrowserContextKeyedServiceFactoriesBuilt() {
  hns_adaptive_captcha::HnsAdaptiveCaptchaServiceFactory::GetInstance();
  hns_ads::AdsServiceFactory::GetInstance();
  hns_federated::HnsFederatedServiceFactory::GetInstance();
  hns_perf_predictor::NamedThirdPartyRegistryFactory::GetInstance();
  hns_rewards::RewardsServiceFactory::GetInstance();
  hns_shields::AdBlockPrefServiceFactory::GetInstance();
  debounce::DebounceServiceFactory::GetInstance();
  hns::URLSanitizerServiceFactory::GetInstance();
  HnsRendererUpdaterFactory::GetInstance();
  SearchEngineProviderServiceFactory::GetInstance();
  misc_metrics::PageMetricsServiceFactory::GetInstance();
#if BUILDFLAG(ENABLE_GREASELION)
  greaselion::GreaselionServiceFactory::GetInstance();
#endif
#if BUILDFLAG(ENABLE_TOR)
  TorProfileServiceFactory::GetInstance();
#endif
  SearchEngineTrackerFactory::GetInstance();
  ntp_background_images::ViewCounterServiceFactory::GetInstance();

#if !BUILDFLAG(IS_ANDROID)
  BookmarkPrefsServiceFactory::GetInstance();
#else
  hns_shields::CookieListOptInServiceFactory::GetInstance();
  hns_shields::FilterListServiceFactory::GetInstance();
  ntp_background_images::NTPBackgroundImagesBridgeFactory::GetInstance();
#endif

  hns_news::HnsNewsControllerFactory::GetInstance();
  hns_wallet::AssetRatioServiceFactory::GetInstance();
  hns_wallet::KeyringServiceFactory::GetInstance();
  hns_wallet::JsonRpcServiceFactory::GetInstance();
  hns_wallet::SwapServiceFactory::GetInstance();
  hns_wallet::SimulationServiceFactory::GetInstance();
#if !BUILDFLAG(IS_ANDROID)
  hns_wallet::WalletNotificationServiceFactory::GetInstance();
#endif
  hns_wallet::TxServiceFactory::GetInstance();
  hns_wallet::HnsWalletServiceFactory::GetInstance();

#if !BUILDFLAG(IS_ANDROID)
  if (base::FeatureList::IsEnabled(commands::features::kHnsCommands)) {
    commands::AcceleratorServiceFactory::GetInstance();
  }
#endif

#if BUILDFLAG(ENABLE_COMMANDER)
  if (base::FeatureList::IsEnabled(features::kHnsCommander)) {
    commander::CommanderServiceFactory::GetInstance();
  }
#endif

#if BUILDFLAG(ETHEREUM_REMOTE_CLIENT_ENABLED)
  EthereumRemoteClientServiceFactory::GetInstance();
#endif

#if BUILDFLAG(ENABLE_IPFS)
  ipfs::IpfsServiceFactory::GetInstance();
#endif
  hns_wallet::HnsWalletIpfsServiceFactory::GetInstance();

#if BUILDFLAG(ENABLE_IPFS_LOCAL_NODE)
  hns_wallet::HnsWalletAutoPinServiceFactory::GetInstance();
  hns_wallet::HnsWalletPinServiceFactory::GetInstance();
#endif

  EphemeralStorageServiceFactory::GetInstance();
  PermissionLifetimeManagerFactory::GetInstance();
#if BUILDFLAG(ENABLE_HNS_VPN)
  skus::SkusServiceFactory::GetInstance();
  hns_vpn::HnsVpnServiceFactory::GetInstance();
#endif
#if BUILDFLAG(ENABLE_PLAYLIST)
  if (base::FeatureList::IsEnabled(playlist::features::kPlaylist)) {
    playlist::PlaylistServiceFactory::GetInstance();
  }
#endif
#if BUILDFLAG(ENABLE_REQUEST_OTR)
  request_otr::RequestOTRServiceFactory::GetInstance();
#endif

  HnsSyncAlertsServiceFactory::GetInstance();

#if !BUILDFLAG(IS_ANDROID)
  if (base::FeatureList::IsEnabled(tabs::features::kHnsSharedPinnedTabs)) {
    SharedPinnedTabServiceFactory::GetInstance();
  }
#endif

#if defined(TOOLKIT_VIEWS)
  sidebar::SidebarServiceFactory::GetInstance();
#endif

#if BUILDFLAG(ENABLE_SPEEDREADER)
  speedreader::SpeedreaderServiceFactory::GetInstance();
#endif
}

}  // namespace hns
