/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_SHIELDS_COMMON_HNS_SHIELD_LOCALIZED_STRINGS_H_
#define HNS_COMPONENTS_HNS_SHIELDS_COMMON_HNS_SHIELD_LOCALIZED_STRINGS_H_

#include "components/grit/hns_components_strings.h"
#include "ui/base/webui/web_ui_util.h"

namespace hns_shields {

constexpr webui::LocalizedString kLocalizedStrings[] = {
    {"hnsShields", IDS_HNS_SHIELDS},
    {"hnsShieldsStandalone", IDS_HNS_SHIELDS_STANDALONE},
    {"hnsShieldsEnable", IDS_HNS_SHIELDS_ENABLE},
    {"hnsShieldsUp", IDS_HNS_SHIELDS_UP},
    {"hnsShieldsDown", IDS_HNS_SHIELDS_DOWN},
    {"hnsShieldsBroken", IDS_HNS_SHIELDS_BROKEN},
    {"hnsShieldsBlockedNote", IDS_HNS_SHIELDS_BLOCKED_NOTE},
    {"hnsShieldsNOTBlockedNote", IDS_HNS_SHIELDS_NOT_BLOCKED_NOTE},
    {"hnsShieldsAdvancedCtrls", IDS_HNS_SHIELDS_ADVANCED_CTRLS},
    {"hnsShieldSettingsDescription", IDS_HNS_SHIELD_SETTINGS_DESCRIPTION},
    {"hnsShieldsGlobalSettingsTitle",
     IDS_HNS_SHIELDS_GLOBAL_SETTINGS_TITLE},
    {"hnsShieldsChangeDefaults", IDS_HNS_SHIELDS_CHANGE_DEFAULTS},
    {"hnsShieldsCustomizeAdblockLists",
     IDS_HNS_SHIELDS_CUSTOMIZE_ADBLOCK_LISTS},
    {"hnsShieldsConnectionsUpgraded", IDS_HNS_SHIELDS_CONNECTIONS_UPGRADED},
    {"hnsShieldsHTTPSEnable", IDS_HNS_SHIELDS_HTTPS_ENABLE},
    {"hnsShieldsScriptsBlocked", IDS_HNS_SHIELDS_SCRIPTS_BLOCKED},
    {"hnsShieldsScriptsBlockedEnable",
     IDS_HNS_SHIELDS_SCRIPTS_BLOCKED_ENABLE},
    {"hnsShieldsTrackersAndAds", IDS_HNS_SHIELDS_TRACKERS_AND_ADS},
    {"hnsShieldsTrackersAndAdsBlockedStd",
     IDS_HNS_SHIELDS_TRACKERS_AND_ADS_BLOCKED_STD},
    {"hnsShieldsTrackersAndAdsBlockedAgg",
     IDS_HNS_SHIELDS_TRACKERS_AND_ADS_BLOCKED_AGG},
    {"hnsShieldsTrackersAndAdsAllowAll",
     IDS_HNS_SHIELDS_TRACKERS_AND_ADS_ALLOW_ALL},
    {"hnsShieldsCrossCookiesBlocked",
     IDS_HNS_SHIELDS_CROSS_COOKIES_BLOCKED},
    {"hnsShieldsForgetFirstPartyStorage",
     IDS_HNS_SHIELDS_FORGET_FIRST_PARTY_STORAGE_LABEL},
    {"hnsShieldsCookiesBlockAll", IDS_HNS_SHIELDS_COOKIES_BLOCKED},
    {"hnsShieldsCookiesAllowedAll", IDS_HNS_SHIELDS_COOKIES_ALLOWED_ALL},
    {"hnsShieldsFingerprintingBlocked",
     IDS_HNS_SHIELDS_FINGERPRINTING_BLOCKED},
    {"hnsShieldsFingerprintingBlockedStd",
     IDS_HNS_SHIELDS_FINGERPRINTING_BLOCKED_STD},
    {"hnsShieldsFingerprintingBlockedAgg",
     IDS_HNS_SHIELDS_FINGERPRINTING_BLOCKED_AGG},
    {"hnsShieldsFingerprintingAllowAll",
     IDS_HNS_SHIELDS_FINGERPRINTING_ALLOW_ALL},
    {"hnsShieldsHttpsUpgradeModeDisabled",
     IDS_HNS_SHIELDS_HTTPS_UPGRADE_MODE_DISABLED},
    {"hnsShieldsHttpsUpgradeModeStandard",
     IDS_HNS_SHIELDS_HTTPS_UPGRADE_MODE_STANDARD},
    {"hnsShieldsHttpsUpgradeModeStrict",
     IDS_HNS_SHIELDS_HTTPS_UPGRADE_MODE_STRICT},
    {"hnsShieldsReportSite", IDS_HNS_SHIELDS_REPORT_SITE},
    {"hnsShieldsReportSiteDesc", IDS_HNS_SHIELDS_REPORT_SITE_DESC},
    {"hnsShieldsDownDesc", IDS_HNS_SHIELDS_DOWN_DESC},
    {"hnsShieldsBlockedScriptsLabel",
     IDS_HNS_SHIELDS_BLOCKED_SCRIPTS_LABEL},
    {"hnsShieldsAllowedScriptsLabel",
     IDS_HNS_SHIELDS_ALLOWED_SCRIPTS_LABEL},
    {"hnsShieldsManaged", IDS_HNS_SHIELDS_MANAGED},
    {"hnsShieldsAllowScriptOnce", IDS_HNS_SHIELDS_ALLOW_SCRIPT_ONCE},
    {"hnsShieldsBlockScript", IDS_HNS_SHIELDS_SCRIPT_BLOCK},
    {"hnsShieldsAllowScriptsAll", IDS_HNS_SHIELDS_ALLOW_SCRIPTS_ALL},
    {"hnsShieldsBlockScriptsAll", IDS_HNS_SHIELDS_BLOCK_SCRIPTS_ALL}};

}  // namespace hns_shields

#endif  // HNS_COMPONENTS_HNS_SHIELDS_COMMON_HNS_SHIELD_LOCALIZED_STRINGS_H_
