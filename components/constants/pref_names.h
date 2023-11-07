/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_CONSTANTS_PREF_NAMES_H_
#define HNS_COMPONENTS_CONSTANTS_PREF_NAMES_H_

#include "build/build_config.h"

extern const char kHnsAutofillPrivateWindows[];
extern const char kManagedHnsShieldsEnabledForUrls[];
extern const char kManagedHnsShieldsDisabledForUrls[];
extern const char kAdsBlocked[];
extern const char kTrackersBlocked[];
extern const char kJavascriptBlocked[];
extern const char kHttpsUpgrades[];
extern const char kFingerprintingBlocked[];
extern const char kLastCheckYMD[];
extern const char kLastCheckWOY[];
extern const char kLastCheckMonth[];
extern const char kFirstCheckMade[];
extern const char kThresholdCheckMade[];
extern const char kThresholdQuery[];
extern const char kWeekOfInstallation[];
extern const char kStatsReportingEnabled[];
extern const char kWidevineOptedIn[];
extern const char kAskWidevineInstall[];
extern const char kHnsThemeType[];
extern const char kUseOverriddenHnsThemeType[];
extern const char kShowBookmarksButton[];
extern const char kShowSidePanelButton[];
extern const char kLocationBarIsWide[];
extern const char kReferralDownloadID[];
extern const char kReferralTimestamp[];
extern const char kReferralAttemptTimestamp[];
extern const char kReferralAttemptCount[];
extern const char kReferralHeaders[];
extern const char kReferralAndroidFirstRunTimestamp[];
extern const char kNoScriptControlType[];
extern const char kShieldsAdvancedViewEnabled[];
extern const char kShieldsStatsBadgeVisible[];
extern const char kAdControlType[];
extern const char kGoogleLoginControlType[];
extern const char kWebTorrentEnabled[];
extern const char kHangoutsEnabled[];
extern const char kIPFSCompanionEnabled[];
extern const char kNewTabPageShowClock[];
extern const char kNewTabPageClockFormat[];
extern const char kNewTabPageShowTopSites[];
extern const char kNewTabPageShowStats[];
extern const char kNewTabPageShowRewards[];
extern const char kNewTabPageShowHnsTalk[];
extern const char kNewTabPageHideAllWidgets[];
extern const char kNewTabPageShowsOptions[];
extern const char kHnsNewsIntroDismissed[];
extern const char kAlwaysShowBookmarkBarOnNTP[];
extern const char kHnsDarkMode[];
extern const char kOtherBookmarksMigrated[];
extern const char kHnsShieldsSettingsVersion[];
extern const char kDefaultBrowserPromptEnabled[];

// Web discovery project
extern const char kWebDiscoveryEnabled[];
extern const char kWebDiscoveryCTAState[];
extern const char kDontAskEnableWebDiscovery[];  // Deprecated.
extern const char kHnsSearchVisitCount[];      // Deprecated.

extern const char kHnsGCMChannelStatus[];
extern const char kImportDialogExtensions[];
extern const char kImportDialogPayments[];
extern const char kMRUCyclingEnabled[];

#if BUILDFLAG(IS_ANDROID)
extern const char kDesktopModeEnabled[];
extern const char kPlayYTVideoInBrowserEnabled[];
extern const char kBackgroundVideoPlaybackEnabled[];
extern const char kSafetynetCheckFailed[];
extern const char kSafetynetStatus[];
#endif

#if !BUILDFLAG(IS_ANDROID)
extern const char kEnableWindowClosingConfirm[];
extern const char kEnableClosingLastTab[];
#endif
extern const char kDefaultBrowserLaunchingCount[];
extern const char kTabsSearchShow[];
extern const char kTabMuteIndicatorNotClickable[];
extern const char kDontAskForCrashReporting[];

// Cast extension requires a browser restart once the setting is toggled.
// kEnableMediaRouterOnRestart is used as a proxy to identify the current
// state of the switch and prefs::kEnableMediaRouter is updated to
// kEnableMediaRouterOnRestart on restart.
extern const char kEnableMediaRouterOnRestart[];

// Obsolete widget removal prefs
#if !BUILDFLAG(IS_IOS) && !BUILDFLAG(IS_ANDROID)
constexpr char kFTXAccessToken[] = "hns.ftx.access_token";
constexpr char kFTXOauthHost[] = "hns.ftx.oauth_host";
constexpr char kFTXNewTabPageShowFTX[] = "ftx.new_tab_page.show_ftx";
constexpr char kCryptoDotComNewTabPageShowCryptoDotCom[] =
    "crypto_dot_com.new_tab_page.show_crypto_dot_com";
constexpr char kCryptoDotComHasBoughtCrypto[] =
    "crypto_dot_com.new_tab_page.has_bought_crypto";
constexpr char kCryptoDotComHasInteracted[] =
    "crypto_dot_com.new_tab_page.has_interacted";
constexpr char kGeminiAccessToken[] = "hns.gemini.access_token";
constexpr char kGeminiRefreshToken[] = "hns.gemini.refresh_token";
constexpr char kNewTabPageShowGemini[] = "hns.new_tab_page.show_gemini";
#endif

#if !BUILDFLAG(IS_IOS)
constexpr char kBinanceAccessToken[] = "hns.binance.access_token";
constexpr char kBinanceRefreshToken[] = "hns.binance.refresh_token";
constexpr char kNewTabPageShowBinance[] = "hns.new_tab_page.show_binance";
constexpr char kHnsSuggestedSiteSuggestionsEnabled[] =
    "hns.hns_suggested_site_suggestions_enabled";
#endif

#endif  // HNS_COMPONENTS_CONSTANTS_PREF_NAMES_H_
