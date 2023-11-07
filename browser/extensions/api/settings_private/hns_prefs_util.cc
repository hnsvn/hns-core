/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/browser/extensions/api/settings_private/hns_prefs_util.h"

#include "base/feature_list.h"
#include "hns/browser/ethereum_remote_client/buildflags/buildflags.h"
#include "hns/browser/ui/tabs/hns_tab_prefs.h"
#include "hns/browser/ui/tabs/features.h"
#include "hns/components/ai_chat/common/buildflags/buildflags.h"
#include "hns/components/hns_ads/core/public/prefs/pref_names.h"
#include "hns/components/hns_news/common/pref_names.h"
#include "hns/components/hns_rewards/common/pref_names.h"
#include "hns/components/hns_shields/common/pref_names.h"
#include "hns/components/hns_vpn/common/buildflags/buildflags.h"
#include "hns/components/hns_wallet/browser/pref_names.h"
#include "hns/components/hns_wayback_machine/buildflags/buildflags.h"
#include "hns/components/constants/pref_names.h"
#include "hns/components/de_amp/common/pref_names.h"
#include "hns/components/debounce/common/pref_names.h"
#include "hns/components/decentralized_dns/core/pref_names.h"
#include "hns/components/ipfs/buildflags/buildflags.h"
#include "hns/components/ntp_background_images/common/pref_names.h"
#include "hns/components/omnibox/browser/hns_omnibox_prefs.h"
#include "hns/components/request_otr/common/pref_names.h"
#include "hns/components/speedreader/common/buildflags/buildflags.h"
#include "hns/components/tor/buildflags/buildflags.h"
#include "chrome/browser/extensions/api/settings_private/prefs_util.h"
#include "chrome/common/extensions/api/settings_private.h"
#include "chrome/common/pref_names.h"
#include "components/browsing_data/core/pref_names.h"
#include "components/gcm_driver/gcm_buildflags.h"
#include "components/omnibox/browser/omnibox_prefs.h"
#include "components/search_engines/search_engines_pref_names.h"
#include "extensions/buildflags/buildflags.h"

#if BUILDFLAG(ENABLE_HNS_WAYBACK_MACHINE)
#include "hns/components/hns_wayback_machine/pref_names.h"
#endif

#if BUILDFLAG(ETHEREUM_REMOTE_CLIENT_ENABLED)
#include "hns/browser/ethereum_remote_client/pref_names.h"
#endif

#if BUILDFLAG(ENABLE_IPFS)
#include "hns/components/ipfs/pref_names.h"
#endif

#if BUILDFLAG(ENABLE_AI_CHAT)
#include "hns/components/ai_chat/common/pref_names.h"
#endif

#if defined(TOOLKIT_VIEWS)
#include "hns/components/sidebar/pref_names.h"
#endif

#if BUILDFLAG(ENABLE_SPEEDREADER)
#include "hns/components/speedreader/speedreader_pref_names.h"
#endif

#if BUILDFLAG(ENABLE_TOR)
#include "hns/components/tor/pref_names.h"
#endif

#if BUILDFLAG(ENABLE_HNS_VPN)
#include "hns/components/hns_vpn/common/pref_names.h"
#endif

namespace extensions {

using ntp_background_images::prefs::kNewTabPageShowBackgroundImage;
using ntp_background_images::prefs::
    kNewTabPageShowSponsoredImagesBackgroundImage;
using ntp_background_images::prefs::kNewTabPageSuperReferralThemesOption;

namespace settings_api = api::settings_private;

const PrefsUtil::TypedPrefMap& HnsPrefsUtil::GetAllowlistedKeys() {
  // Static cache, similar to parent class
  static PrefsUtil::TypedPrefMap* s_hns_allowlist = nullptr;
  if (s_hns_allowlist)
    return *s_hns_allowlist;
  s_hns_allowlist = new PrefsUtil::TypedPrefMap();
  // Start with parent class allowlist
  const auto chromium_prefs = PrefsUtil::GetAllowlistedKeys();
  s_hns_allowlist->insert(chromium_prefs.begin(), chromium_prefs.end());
  // Add Hns values to the allowlist
  // import data
  (*s_hns_allowlist)[kImportDialogExtensions] =
      settings_api::PrefType::PREF_TYPE_BOOLEAN;
  (*s_hns_allowlist)[kImportDialogPayments] =
      settings_api::PrefType::PREF_TYPE_BOOLEAN;
  // Default Hns shields
  (*s_hns_allowlist)[kShieldsAdvancedViewEnabled] =
      settings_api::PrefType::PREF_TYPE_BOOLEAN;
  (*s_hns_allowlist)[kShieldsStatsBadgeVisible] =
      settings_api::PrefType::PREF_TYPE_BOOLEAN;
  (*s_hns_allowlist)[kAdControlType] =
      settings_api::PrefType::PREF_TYPE_BOOLEAN;
  (*s_hns_allowlist)[kNoScriptControlType] =
      settings_api::PrefType::PREF_TYPE_BOOLEAN;
  (*s_hns_allowlist)[kGoogleLoginControlType] =
      settings_api::PrefType::PREF_TYPE_BOOLEAN;
  (*s_hns_allowlist)[hns_shields::prefs::kFBEmbedControlType] =
      settings_api::PrefType::PREF_TYPE_BOOLEAN;
  (*s_hns_allowlist)[hns_shields::prefs::kTwitterEmbedControlType] =
      settings_api::PrefType::PREF_TYPE_BOOLEAN;
  (*s_hns_allowlist)[hns_shields::prefs::kLinkedInEmbedControlType] =
      settings_api::PrefType::PREF_TYPE_BOOLEAN;
  (*s_hns_allowlist)[hns_shields::prefs::kReduceLanguageEnabled] =
      settings_api::PrefType::PREF_TYPE_BOOLEAN;

  // Rewards/Ads prefs
  (*s_hns_allowlist)[hns_rewards::prefs::kEnabled] =
      settings_api::PrefType::PREF_TYPE_BOOLEAN;
  (*s_hns_allowlist)[hns_rewards::prefs::kShowLocationBarButton] =
      settings_api::PrefType::PREF_TYPE_BOOLEAN;
  (*s_hns_allowlist)[hns_rewards::prefs::kInlineTipButtonsEnabled] =
      settings_api::PrefType::PREF_TYPE_BOOLEAN;
  (*s_hns_allowlist)[hns_rewards::prefs::kInlineTipRedditEnabled] =
      settings_api::PrefType::PREF_TYPE_BOOLEAN;
  (*s_hns_allowlist)[hns_rewards::prefs::kInlineTipTwitterEnabled] =
      settings_api::PrefType::PREF_TYPE_BOOLEAN;
  (*s_hns_allowlist)[hns_rewards::prefs::kInlineTipGithubEnabled] =
      settings_api::PrefType::PREF_TYPE_BOOLEAN;

  // Search engine prefs
  (*s_hns_allowlist)[prefs::kAddOpenSearchEngines] =
      settings_api::PrefType::PREF_TYPE_BOOLEAN;
  (*s_hns_allowlist)[prefs::kSyncedDefaultPrivateSearchProviderGUID] =
      settings_api::PrefType::PREF_TYPE_NUMBER;

  // autofill prefs
  (*s_hns_allowlist)[kHnsAutofillPrivateWindows] =
      settings_api::PrefType::PREF_TYPE_BOOLEAN;

  // appearance prefs
  (*s_hns_allowlist)[kShowBookmarksButton] =
      settings_api::PrefType::PREF_TYPE_BOOLEAN;
  (*s_hns_allowlist)[kShowSidePanelButton] =
      settings_api::PrefType::PREF_TYPE_BOOLEAN;
  (*s_hns_allowlist)[hns_news::prefs::kShouldShowToolbarButton] =
      settings_api::PrefType::PREF_TYPE_BOOLEAN;
  (*s_hns_allowlist)[kLocationBarIsWide] =
      settings_api::PrefType::PREF_TYPE_BOOLEAN;
  (*s_hns_allowlist)[omnibox::kAutocompleteEnabled] =
      settings_api::PrefType::PREF_TYPE_BOOLEAN;
  (*s_hns_allowlist)[omnibox::kTopSiteSuggestionsEnabled] =
      settings_api::PrefType::PREF_TYPE_BOOLEAN;
  (*s_hns_allowlist)[omnibox::kHistorySuggestionsEnabled] =
      settings_api::PrefType::PREF_TYPE_BOOLEAN;
  (*s_hns_allowlist)[omnibox::kBookmarkSuggestionsEnabled] =
      settings_api::PrefType::PREF_TYPE_BOOLEAN;
  (*s_hns_allowlist)[kAskWidevineInstall] =
      settings_api::PrefType::PREF_TYPE_BOOLEAN;
  (*s_hns_allowlist)[kNewTabPageSuperReferralThemesOption] =
      settings_api::PrefType::PREF_TYPE_NUMBER;
  (*s_hns_allowlist)[kTabsSearchShow] =
      settings_api::PrefType::PREF_TYPE_BOOLEAN;
  (*s_hns_allowlist)[hns_tabs::kTabHoverMode] =
      settings_api::PREF_TYPE_NUMBER;
  (*s_hns_allowlist)[kTabMuteIndicatorNotClickable] =
      settings_api::PrefType::PREF_TYPE_BOOLEAN;
#if BUILDFLAG(ENABLE_HNS_VPN)
  (*s_hns_allowlist)[hns_vpn::prefs::kHnsVPNShowButton] =
      settings_api::PrefType::PREF_TYPE_BOOLEAN;
#if BUILDFLAG(ENABLE_HNS_VPN_WIREGUARD)
  (*s_hns_allowlist)[hns_vpn::prefs::kHnsVPNWireguardEnabled] =
      settings_api::PrefType::PREF_TYPE_BOOLEAN;
#endif
#endif
#if defined(TOOLKIT_VIEWS)
  (*s_hns_allowlist)[sidebar::kSidebarShowOption] =
      settings_api::PrefType::PREF_TYPE_NUMBER;
#endif
#if BUILDFLAG(ENABLE_SPEEDREADER)
  (*s_hns_allowlist)[speedreader::kSpeedreaderPrefEnabled] =
      settings_api::PrefType::PREF_TYPE_BOOLEAN;
#endif
  // De-AMP feature
  (*s_hns_allowlist)[de_amp::kDeAmpPrefEnabled] =
      settings_api::PrefType::PREF_TYPE_BOOLEAN;
  // Debounce feature
  (*s_hns_allowlist)[debounce::prefs::kDebounceEnabled] =
      settings_api::PrefType::PREF_TYPE_BOOLEAN;
  // new tab prefs
  (*s_hns_allowlist)[kNewTabPageShowSponsoredImagesBackgroundImage] =
      settings_api::PrefType::PREF_TYPE_BOOLEAN;
  (*s_hns_allowlist)[kNewTabPageShowBackgroundImage] =
      settings_api::PrefType::PREF_TYPE_BOOLEAN;
  (*s_hns_allowlist)[kNewTabPageShowClock] =
      settings_api::PrefType::PREF_TYPE_BOOLEAN;
  (*s_hns_allowlist)[kNewTabPageShowStats] =
      settings_api::PrefType::PREF_TYPE_BOOLEAN;
  (*s_hns_allowlist)[kNewTabPageShowRewards] =
      settings_api::PrefType::PREF_TYPE_BOOLEAN;
  (*s_hns_allowlist)[kNewTabPageShowHnsTalk] =
      settings_api::PrefType::PREF_TYPE_BOOLEAN;
  (*s_hns_allowlist)[kNewTabPageShowsOptions] =
      settings_api::PrefType::PREF_TYPE_NUMBER;
#if BUILDFLAG(ENABLE_EXTENSIONS)
  // Web discovery prefs
  (*s_hns_allowlist)[kWebDiscoveryEnabled] =
      settings_api::PrefType::PREF_TYPE_BOOLEAN;
#endif
  // Clear browsing data on exit prefs.
  (*s_hns_allowlist)[browsing_data::prefs::kDeleteBrowsingHistoryOnExit] =
      settings_api::PrefType::PREF_TYPE_BOOLEAN;
  (*s_hns_allowlist)[browsing_data::prefs::kDeleteDownloadHistoryOnExit] =
      settings_api::PrefType::PREF_TYPE_BOOLEAN;
  (*s_hns_allowlist)[browsing_data::prefs::kDeleteCacheOnExit] =
      settings_api::PrefType::PREF_TYPE_BOOLEAN;
  (*s_hns_allowlist)[browsing_data::prefs::kDeleteCookiesOnExit] =
      settings_api::PrefType::PREF_TYPE_BOOLEAN;
  (*s_hns_allowlist)[browsing_data::prefs::kDeletePasswordsOnExit] =
      settings_api::PrefType::PREF_TYPE_BOOLEAN;
  (*s_hns_allowlist)[browsing_data::prefs::kDeleteFormDataOnExit] =
      settings_api::PrefType::PREF_TYPE_BOOLEAN;
  (*s_hns_allowlist)[browsing_data::prefs::kDeleteSiteSettingsOnExit] =
      settings_api::PrefType::PREF_TYPE_BOOLEAN;
  (*s_hns_allowlist)[browsing_data::prefs::kDeleteHostedAppsDataOnExit] =
      settings_api::PrefType::PREF_TYPE_BOOLEAN;
  (*s_hns_allowlist)[kAlwaysShowBookmarkBarOnNTP] =
      settings_api::PrefType::PREF_TYPE_BOOLEAN;
  (*s_hns_allowlist)[kMRUCyclingEnabled] =
      settings_api::PrefType::PREF_TYPE_BOOLEAN;
  // WebTorrent pref
  (*s_hns_allowlist)[kWebTorrentEnabled] =
      settings_api::PrefType::PREF_TYPE_BOOLEAN;
#if BUILDFLAG(ENABLE_HNS_WAYBACK_MACHINE)
  (*s_hns_allowlist)[kHnsWaybackMachineEnabled] =
      settings_api::PrefType::PREF_TYPE_BOOLEAN;
#endif
  (*s_hns_allowlist)[kEnableWindowClosingConfirm] =
      settings_api::PrefType::PREF_TYPE_BOOLEAN;
  (*s_hns_allowlist)[kEnableClosingLastTab] =
      settings_api::PrefType::PREF_TYPE_BOOLEAN;
  // Hangouts pref
  (*s_hns_allowlist)[kHangoutsEnabled] =
      settings_api::PrefType::PREF_TYPE_BOOLEAN;
  // IPFS Companion pref
  (*s_hns_allowlist)[kIPFSCompanionEnabled] =
      settings_api::PrefType::PREF_TYPE_BOOLEAN;

  // Hns Wallet pref
  (*s_hns_allowlist)[kHnsWalletSelectedNetworks] =
      settings_api::PrefType::PREF_TYPE_DICTIONARY;
  (*s_hns_allowlist)[kDefaultEthereumWallet] =
      settings_api::PrefType::PREF_TYPE_NUMBER;
  (*s_hns_allowlist)[kDefaultSolanaWallet] =
      settings_api::PrefType::PREF_TYPE_NUMBER;
  (*s_hns_allowlist)[kDefaultBaseCurrency] =
      settings_api::PrefType::PREF_TYPE_STRING;
  (*s_hns_allowlist)[kDefaultBaseCryptocurrency] =
      settings_api::PrefType::PREF_TYPE_STRING;
  (*s_hns_allowlist)[kShowWalletIconOnToolbar] =
      settings_api::PrefType::PREF_TYPE_BOOLEAN;
  (*s_hns_allowlist)[kHnsWalletAutoLockMinutes] =
      settings_api::PrefType::PREF_TYPE_NUMBER;
  (*s_hns_allowlist)[kHnsWalletNftDiscoveryEnabled] =
      settings_api::PrefType::PREF_TYPE_BOOLEAN;

  // IPFS pref
#if BUILDFLAG(ENABLE_IPFS)
  (*s_hns_allowlist)[kIPFSResolveMethod] =
      settings_api::PrefType::PREF_TYPE_NUMBER;
  (*s_hns_allowlist)[kIPFSAutoFallbackToGateway] =
      settings_api::PrefType::PREF_TYPE_BOOLEAN;
  (*s_hns_allowlist)[kIPFSPublicGatewayAddress] =
      settings_api::PrefType::PREF_TYPE_STRING;
  (*s_hns_allowlist)[kIPFSPublicNFTGatewayAddress] =
      settings_api::PrefType::PREF_TYPE_STRING;
  (*s_hns_allowlist)[kIPFSAutoRedirectToConfiguredGateway] =
      settings_api::PrefType::PREF_TYPE_BOOLEAN;
  (*s_hns_allowlist)[kIpfsStorageMax] =
      settings_api::PrefType::PREF_TYPE_NUMBER;
#endif

// Leo Assistant pref
#if BUILDFLAG(ENABLE_AI_CHAT)
  (*s_hns_allowlist)[ai_chat::prefs::kHnsChatAutoGenerateQuestions] =
      settings_api::PrefType::PREF_TYPE_BOOLEAN;
#endif

#if !BUILDFLAG(USE_GCM_FROM_PLATFORM)
  // Push Messaging Pref
  (*s_hns_allowlist)[kHnsGCMChannelStatus] =
      settings_api::PrefType::PREF_TYPE_BOOLEAN;
#endif
  // Omnibox pref
  (*s_hns_allowlist)[omnibox::kPreventUrlElisionsInOmnibox] =
      settings_api::PrefType::PREF_TYPE_BOOLEAN;
#if BUILDFLAG(ENABLE_TOR)
  (*s_hns_allowlist)[tor::prefs::kAutoOnionRedirect] =
      settings_api::PrefType::PREF_TYPE_BOOLEAN;
  (*s_hns_allowlist)[tor::prefs::kBridgesConfig] =
      settings_api::PrefType::PREF_TYPE_DICTIONARY;
#endif
  (*s_hns_allowlist)[prefs::kWebRTCIPHandlingPolicy] =
      settings_api::PrefType::PREF_TYPE_STRING;
  // Request OTR feature
  (*s_hns_allowlist)[request_otr::kRequestOTRActionOption] =
      settings_api::PrefType::PREF_TYPE_NUMBER;

  (*s_hns_allowlist)[decentralized_dns::kUnstoppableDomainsResolveMethod] =
      settings_api::PrefType::PREF_TYPE_NUMBER;
  (*s_hns_allowlist)[decentralized_dns::kENSResolveMethod] =
      settings_api::PrefType::PREF_TYPE_NUMBER;
  (*s_hns_allowlist)[decentralized_dns::kEnsOffchainResolveMethod] =
      settings_api::PrefType::PREF_TYPE_NUMBER;
  (*s_hns_allowlist)[decentralized_dns::kSnsResolveMethod] =
      settings_api::PrefType::PREF_TYPE_NUMBER;

  // Media router pref
  (*s_hns_allowlist)[kEnableMediaRouterOnRestart] =
      settings_api::PrefType::PREF_TYPE_BOOLEAN;

  // NFT pinning pref
  (*s_hns_allowlist)[kAutoPinEnabled] =
      settings_api::PrefType::PREF_TYPE_BOOLEAN;

#if defined(TOOLKIT_VIEWS)
  // Vertical tab strip prefs
  if (base::FeatureList::IsEnabled(tabs::features::kHnsVerticalTabs)) {
    (*s_hns_allowlist)[hns_tabs::kVerticalTabsEnabled] =
        settings_api::PrefType::PREF_TYPE_BOOLEAN;
    (*s_hns_allowlist)[hns_tabs::kVerticalTabsFloatingEnabled] =
        settings_api::PrefType::PREF_TYPE_BOOLEAN;
    (*s_hns_allowlist)[hns_tabs::kVerticalTabsShowTitleOnWindow] =
        settings_api::PrefType::PREF_TYPE_BOOLEAN;
  }
#endif
  return *s_hns_allowlist;
}

}  // namespace extensions
