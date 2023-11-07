/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "base/feature_list.h"
#include "hns/browser/ethereum_remote_client/buildflags/buildflags.h"
#include "hns/browser/metrics/buildflags/buildflags.h"
#include "hns/components/hns_rewards/common/pref_names.h"
#include "hns/components/hns_shields/common/features.h"
#include "hns/components/hns_shields/common/pref_names.h"
#include "hns/components/hns_vpn/common/buildflags/buildflags.h"
#include "hns/components/hns_wallet/browser/hns_wallet_utils.h"
#include "hns/components/hns_wallet/browser/pref_names.h"
#include "hns/components/hns_wallet/common/hns_wallet.mojom.h"
#include "hns/components/hns_wayback_machine/buildflags/buildflags.h"
#include "hns/components/constants/pref_names.h"
#include "hns/components/ipfs/buildflags/buildflags.h"
#include "hns/components/ntp_background_images/buildflags/buildflags.h"
#include "build/build_config.h"
#include "chrome/browser/browser_process.h"
#include "chrome/browser/prefetch/pref_names.h"
#include "chrome/browser/prefetch/prefetch_prefs.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/ui/webui/new_tab_page/ntp_pref_names.h"
#include "chrome/common/pref_names.h"
#include "chrome/test/base/chrome_test_utils.h"
#include "components/embedder_support/pref_names.h"
#include "components/gcm_driver/gcm_buildflags.h"
#include "components/policy/core/common/policy_pref_names.h"
#include "components/prefs/pref_service.h"
#include "components/safe_browsing/core/common/safe_browsing_prefs.h"
#include "components/signin/public/base/signin_pref_names.h"
#include "components/spellcheck/browser/pref_names.h"
#include "components/sync/base/pref_names.h"
#include "content/public/test/browser_test.h"

#if BUILDFLAG(ENABLE_IPFS)
#include "hns/components/ipfs/ipfs_constants.h"
#include "hns/components/ipfs/pref_names.h"
#endif

#if BUILDFLAG(ETHEREUM_REMOTE_CLIENT_ENABLED)
#include "hns/browser/ethereum_remote_client/pref_names.h"
#endif

#if BUILDFLAG(ENABLE_HNS_WAYBACK_MACHINE)
#include "hns/components/hns_wayback_machine/pref_names.h"
#endif

#if BUILDFLAG(ENABLE_HNS_VPN)
#include "hns/components/hns_vpn/common/pref_names.h"
#endif

#if BUILDFLAG(IS_ANDROID)
#include "chrome/test/base/android/android_browser_test.h"
#else
#include "chrome/test/base/in_process_browser_test.h"
#endif

#if BUILDFLAG(ENABLE_CUSTOM_BACKGROUND)
#include "hns/browser/ntp_background/ntp_background_prefs.h"
#endif

using HnsProfilePrefsBrowserTest = PlatformBrowserTest;
using HnsLocalStatePrefsBrowserTest = PlatformBrowserTest;

// Check download prompt preference is set to true by default.
IN_PROC_BROWSER_TEST_F(HnsProfilePrefsBrowserTest, DownloadPromptDefault) {
  EXPECT_TRUE(chrome_test_utils::GetProfile(this)->GetPrefs()->GetBoolean(
      prefs::kPromptForDownload));
}

IN_PROC_BROWSER_TEST_F(HnsProfilePrefsBrowserTest, MiscHnsPrefs) {
  EXPECT_FALSE(chrome_test_utils::GetProfile(this)->GetPrefs()->GetBoolean(
      kNoScriptControlType));
  EXPECT_FALSE(chrome_test_utils::GetProfile(this)->GetPrefs()->GetBoolean(
      kShieldsAdvancedViewEnabled));
  EXPECT_TRUE(chrome_test_utils::GetProfile(this)->GetPrefs()->GetBoolean(
      kShieldsStatsBadgeVisible));
  EXPECT_TRUE(chrome_test_utils::GetProfile(this)->GetPrefs()->GetBoolean(
      kAdControlType));
  EXPECT_TRUE(chrome_test_utils::GetProfile(this)->GetPrefs()->GetBoolean(
      kGoogleLoginControlType));
  EXPECT_TRUE(chrome_test_utils::GetProfile(this)->GetPrefs()->GetBoolean(
      hns_shields::prefs::kFBEmbedControlType));
  EXPECT_TRUE(chrome_test_utils::GetProfile(this)->GetPrefs()->GetBoolean(
      hns_shields::prefs::kTwitterEmbedControlType));
  EXPECT_FALSE(chrome_test_utils::GetProfile(this)->GetPrefs()->GetBoolean(
      hns_shields::prefs::kLinkedInEmbedControlType));
  EXPECT_EQ(chrome_test_utils::GetProfile(this)->GetPrefs()->GetBoolean(
                hns_shields::prefs::kReduceLanguageEnabled),
            base::FeatureList::IsEnabled(
                hns_shields::features::kHnsReduceLanguage));
#if BUILDFLAG(ENABLE_HNS_WAYBACK_MACHINE)
  EXPECT_TRUE(chrome_test_utils::GetProfile(this)->GetPrefs()->GetBoolean(
      kWebTorrentEnabled));
  EXPECT_TRUE(chrome_test_utils::GetProfile(this)->GetPrefs()->GetBoolean(
      kHnsWaybackMachineEnabled));
#endif
  EXPECT_TRUE(chrome_test_utils::GetProfile(this)->GetPrefs()->GetBoolean(
      kHangoutsEnabled));
  EXPECT_TRUE(chrome_test_utils::GetProfile(this)->GetPrefs()->GetBoolean(
      hns_rewards::prefs::kShowLocationBarButton));
#if BUILDFLAG(ENABLE_IPFS)
  EXPECT_EQ(chrome_test_utils::GetProfile(this)->GetPrefs()->GetInteger(
                kIPFSResolveMethod),
            static_cast<int>((ipfs::IPFSResolveMethodTypes::IPFS_ASK)));
  EXPECT_TRUE(chrome_test_utils::GetProfile(this)
                  ->GetPrefs()
                  ->GetFilePath(kIPFSBinaryPath)
                  .empty());
  EXPECT_FALSE(chrome_test_utils::GetProfile(this)->GetPrefs()->GetBoolean(
      kIPFSAutoRedirectToConfiguredGateway));
#endif
  EXPECT_FALSE(chrome_test_utils::GetProfile(this)->GetPrefs()->GetBoolean(
      kIPFSCompanionEnabled));
#if BUILDFLAG(ETHEREUM_REMOTE_CLIENT_ENABLED)
  EXPECT_FALSE(chrome_test_utils::GetProfile(this)->GetPrefs()->GetBoolean(
      kERCOptedIntoCryptoWallets));
#endif
  EXPECT_EQ(hns_wallet::GetDefaultEthereumWallet(
                chrome_test_utils::GetProfile(this)->GetPrefs()),
            hns_wallet::mojom::DefaultWallet::HnsWalletPreferExtension);
  EXPECT_EQ(hns_wallet::GetDefaultSolanaWallet(
                chrome_test_utils::GetProfile(this)->GetPrefs()),
            hns_wallet::mojom::DefaultWallet::HnsWalletPreferExtension);
  EXPECT_TRUE(chrome_test_utils::GetProfile(this)->GetPrefs()->GetBoolean(
      kShowWalletIconOnToolbar));
  EXPECT_FALSE(chrome_test_utils::GetProfile(this)->GetPrefs()->GetBoolean(
      kMRUCyclingEnabled));
#if !BUILDFLAG(USE_GCM_FROM_PLATFORM)
  EXPECT_FALSE(chrome_test_utils::GetProfile(this)->GetPrefs()->GetBoolean(
      kHnsGCMChannelStatus));
#endif

#if BUILDFLAG(ENABLE_HNS_VPN)
  EXPECT_TRUE(chrome_test_utils::GetProfile(this)->GetPrefs()->GetBoolean(
      hns_vpn::prefs::kHnsVPNShowButton));
#endif

#if BUILDFLAG(ENABLE_CUSTOM_BACKGROUND)
  EXPECT_FALSE(chrome_test_utils::GetProfile(this)->GetPrefs()->HasPrefPath(
      NTPBackgroundPrefs::kDeprecatedPrefName));
#endif
}

IN_PROC_BROWSER_TEST_F(HnsProfilePrefsBrowserTest,
                       DisableGoogleServicesByDefault) {
#if defined(TOOLKIT_VIEWS)
  constexpr char kSideSearchEnabled[] = "side_search.enabled";
  EXPECT_FALSE(chrome_test_utils::GetProfile(this)->GetPrefs()->GetBoolean(
      kSideSearchEnabled));
#endif
  EXPECT_FALSE(chrome_test_utils::GetProfile(this)->GetPrefs()->GetBoolean(
      embedder_support::kAlternateErrorPagesEnabled));
  EXPECT_FALSE(chrome_test_utils::GetProfile(this)->GetPrefs()->GetBoolean(
      spellcheck::prefs::kSpellCheckUseSpellingService));
  EXPECT_FALSE(chrome_test_utils::GetProfile(this)->GetPrefs()->GetBoolean(
      prefs::kSafeBrowsingExtendedReportingOptInAllowed));
  EXPECT_FALSE(chrome_test_utils::GetProfile(this)->GetPrefs()->GetBoolean(
      prefs::kSearchSuggestEnabled));
  EXPECT_EQ(chrome_test_utils::GetProfile(this)->GetPrefs()->GetInteger(
                prefetch::prefs::kNetworkPredictionOptions),
            static_cast<int>(prefetch::NetworkPredictionOptions::kDisabled));
  EXPECT_FALSE(chrome_test_utils::GetProfile(this)->GetPrefs()->GetBoolean(
      prefs::kSigninAllowedOnNextStartup));
  // Verify cloud print is disabled.
  EXPECT_FALSE(chrome_test_utils::GetProfile(this)->GetPrefs()->GetBoolean(
      prefs::kCloudPrintProxyEnabled));
#if !BUILDFLAG(IS_ANDROID)
  EXPECT_TRUE(chrome_test_utils::GetProfile(this)->GetPrefs()->GetBoolean(
      ntp_prefs::kNtpUseMostVisitedTiles));
#endif
  EXPECT_TRUE(chrome_test_utils::GetProfile(this)->GetPrefs()->GetBoolean(
      policy::policy_prefs::kHideWebStoreIcon));
}

IN_PROC_BROWSER_TEST_F(HnsProfilePrefsBrowserTest, MediaRouterPrefTest) {
  EXPECT_FALSE(chrome_test_utils::GetProfile(this)->GetPrefs()->GetBoolean(
      ::prefs::kEnableMediaRouter));
  EXPECT_FALSE(chrome_test_utils::GetProfile(this)->GetPrefs()->GetBoolean(
      kEnableMediaRouterOnRestart));
}

IN_PROC_BROWSER_TEST_F(HnsLocalStatePrefsBrowserTest, DefaultLocalStateTest) {
#if BUILDFLAG(ENABLE_CRASH_DIALOG)
  EXPECT_FALSE(
      g_browser_process->local_state()->GetBoolean(kDontAskForCrashReporting));
#endif
}
