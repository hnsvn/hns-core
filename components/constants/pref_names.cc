/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/components/constants/pref_names.h"

const char kHnsAutofillPrivateWindows[] = "hns.autofill_private_windows";
const char kManagedHnsShieldsDisabledForUrls[] =
    "hns.managed_shields_disabled";
const char kManagedHnsShieldsEnabledForUrls[] =
    "hns.managed_shields_enabled";
const char kAdsBlocked[] = "hns.stats.ads_blocked";
// We no longer update this pref, but we keep it around for now because it's
// added to kAdsBlocked when being displayed.
const char kTrackersBlocked[] = "hns.stats.trackers_blocked";
const char kJavascriptBlocked[] = "hns.stats.javascript_blocked";
const char kHttpsUpgrades[] = "hns.stats.https_upgrades";
const char kFingerprintingBlocked[] = "hns.stats.fingerprinting_blocked";
const char kLastCheckYMD[] = "hns.stats.last_check_ymd";
const char kLastCheckWOY[] = "hns.stats.last_check_woy";
const char kLastCheckMonth[] = "hns.stats.last_check_month";
const char kFirstCheckMade[] = "hns.stats.first_check_made";
// Set to true if the user met the threshold requirements and successfully
// sent a ping to the stats-updater server.
const char kThresholdCheckMade[] = "hns.stats.threshold_check_made";
// Anonymous usage pings enabled
const char kStatsReportingEnabled[] = "hns.stats.reporting_enabled";
// Serialized query for to send to the stats-updater server. Needs to be saved
// in the case that the user sends the standard usage ping, stops the browser,
// meets the threshold requirements, and then starts the browser before the
// threshold ping was sent.
const char kThresholdQuery[] = "hns.stats.threshold_query";
const char kWeekOfInstallation[] = "hns.stats.week_of_installation";
const char kWidevineOptedIn[] = "hns.widevine_opted_in";
const char kAskWidevineInstall[] = "hns.ask_widevine_install";
const char kShowBookmarksButton[] = "hns.show_bookmarks_button";
const char kShowSidePanelButton[] = "hns.show_side_panel_button";
const char kLocationBarIsWide[] = "hns.location_bar_is_wide";
const char kReferralDownloadID[] = "hns.referral.download_id";
const char kReferralTimestamp[] = "hns.referral.timestamp";
const char kReferralAttemptTimestamp[] =
    "hns.referral.referral_attempt_timestamp";
const char kReferralAttemptCount[] = "hns.referral.referral_attempt_count";
const char kReferralHeaders[] = "hns.referral.headers";
const char kReferralAndroidFirstRunTimestamp[] =
    "hns.referral_android_first_run_timestamp";
const char kNoScriptControlType[] = "hns.no_script_default";
const char kShieldsAdvancedViewEnabled[] =
    "hns.shields.advanced_view_enabled";
const char kShieldsStatsBadgeVisible[] = "hns.shields.stats_badge_visible";
const char kAdControlType[] = "hns.ad_default";
const char kGoogleLoginControlType[] = "hns.google_login_default";
const char kWebTorrentEnabled[] = "hns.webtorrent_enabled";
const char kHangoutsEnabled[] = "hns.hangouts_enabled";
const char kIPFSCompanionEnabled[] = "hns.ipfs_companion_enabled";
const char kNewTabPageShowClock[] = "hns.new_tab_page.show_clock";
const char kNewTabPageClockFormat[] = "hns.new_tab_page.clock_format";
const char kNewTabPageShowStats[] = "hns.new_tab_page.show_stats";
const char kNewTabPageShowRewards[] = "hns.new_tab_page.show_rewards";
const char kNewTabPageShowHnsTalk[] = "hns.new_tab_page.show_together";
const char kNewTabPageHideAllWidgets[] = "hns.new_tab_page.hide_all_widgets";
const char kNewTabPageShowsOptions[] = "hns.new_tab_page.shows_options";
const char kHnsNewsIntroDismissed[] = "hns.today.intro_dismissed";
const char kAlwaysShowBookmarkBarOnNTP[] =
    "hns.always_show_bookmark_bar_on_ntp";
const char kHnsDarkMode[] = "hns.dark_mode";
const char kOtherBookmarksMigrated[] = "hns.other_bookmarks_migrated";
const char kHnsShieldsSettingsVersion[] = "hns.shields_settings_version";
const char kDefaultBrowserPromptEnabled[] =
    "hns.default_browser_prompt_enabled";

const char kWebDiscoveryEnabled[] = "hns.web_discovery_enabled";
const char kWebDiscoveryCTAState[] = "hns.web_discovery.cta_state";
const char kDontAskEnableWebDiscovery[] = "hns.dont_ask_enable_web_discovery";
const char kHnsSearchVisitCount[] = "hns.hns_search_visit_count";

const char kHnsGCMChannelStatus[] = "hns.gcm.channel_status";
const char kImportDialogExtensions[] = "import_dialog_extensions";
const char kImportDialogPayments[] = "import_dialog_payments";
const char kMRUCyclingEnabled[] = "hns.mru_cycling_enabled";
const char kTabsSearchShow[] = "hns.tabs_search_show";
const char kTabMuteIndicatorNotClickable[] =
    "hns.tabs.mute_indicator_not_clickable";
const char kDontAskForCrashReporting[] = "hns.dont_ask_for_crash_reporting";
const char kEnableMediaRouterOnRestart[] =
    "hns.enable_media_router_on_restart";

#if BUILDFLAG(IS_ANDROID)
const char kDesktopModeEnabled[] = "hns.desktop_mode_enabled";
const char kPlayYTVideoInBrowserEnabled[] =
    "hns.play_yt_video_in_browser_enabled";
const char kBackgroundVideoPlaybackEnabled[] =
    "hns.background_video_playback";
const char kSafetynetCheckFailed[] = "safetynetcheck.failed";
const char kSafetynetStatus[] = "safetynet.status";
#endif

#if !BUILDFLAG(IS_ANDROID)
const char kEnableWindowClosingConfirm[] =
    "hns.enable_window_closing_confirm";
const char kEnableClosingLastTab[] = "hns.enable_closing_last_tab";
#endif

const char kDefaultBrowserLaunchingCount[] =
    "hns.default_browser.launching_count";

// deprecated
const char kHnsThemeType[] = "hns.theme.type";
const char kUseOverriddenHnsThemeType[] =
    "hns.theme.use_overridden_hns_theme_type";
const char kNewTabPageShowTopSites[] = "hns.new_tab_page.show_top_sites";
