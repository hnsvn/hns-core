/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#define HNS_INIT HnsInit();
#include "src/components/content_settings/core/browser/content_settings_registry.cc"
#undef HNS_INIT

#include "hns/components/hns_shields/common/hns_shield_constants.h"
#include "components/content_settings/core/common/content_settings.h"
#include "net/base/features.h"

namespace content_settings {

void ContentSettingsRegistry::HnsInit() {
  Register(ContentSettingsType::HNS_ADS, hns_shields::kAds,
           CONTENT_SETTING_BLOCK, WebsiteSettingsInfo::SYNCABLE,
           /*allowlisted_schemes=*/{},
           /*valid_settings=*/{CONTENT_SETTING_ALLOW, CONTENT_SETTING_BLOCK},
           WebsiteSettingsInfo::TOP_ORIGIN_ONLY_SCOPE,
           WebsiteSettingsRegistry::DESKTOP |
               WebsiteSettingsRegistry::PLATFORM_ANDROID,
           ContentSettingsInfo::INHERIT_IN_INCOGNITO,
           ContentSettingsInfo::EXCEPTIONS_ON_SECURE_AND_INSECURE_ORIGINS);

  Register(ContentSettingsType::HNS_TRACKERS, hns_shields::kTrackers,
           CONTENT_SETTING_BLOCK, WebsiteSettingsInfo::SYNCABLE,
           /*allowlisted_schemes=*/{},
           /*valid_settings=*/{CONTENT_SETTING_ALLOW, CONTENT_SETTING_BLOCK},
           WebsiteSettingsInfo::TOP_ORIGIN_ONLY_SCOPE,
           WebsiteSettingsRegistry::DESKTOP |
               WebsiteSettingsRegistry::PLATFORM_ANDROID,
           ContentSettingsInfo::INHERIT_IN_INCOGNITO,
           ContentSettingsInfo::EXCEPTIONS_ON_SECURE_AND_INSECURE_ORIGINS);

  Register(ContentSettingsType::HNS_HTTPS_UPGRADE,
           hns_shields::kHTTPSUpgrades, CONTENT_SETTING_ASK,
           WebsiteSettingsInfo::SYNCABLE, /*allowlisted_schemes=*/{},
           /*valid_settings=*/
           {CONTENT_SETTING_ALLOW, CONTENT_SETTING_ASK, CONTENT_SETTING_BLOCK},
           WebsiteSettingsInfo::TOP_ORIGIN_ONLY_SCOPE,
           WebsiteSettingsRegistry::DESKTOP |
               WebsiteSettingsRegistry::PLATFORM_ANDROID,
           ContentSettingsInfo::INHERIT_IF_LESS_PERMISSIVE,
           ContentSettingsInfo::EXCEPTIONS_ON_SECURE_AND_INSECURE_ORIGINS);

  Register(ContentSettingsType::HNS_HTTP_UPGRADABLE_RESOURCES,
           hns_shields::kHTTPUpgradableResources, CONTENT_SETTING_BLOCK,
           WebsiteSettingsInfo::SYNCABLE, /*allowlisted_schemes=*/{},
           /*valid_settings=*/{CONTENT_SETTING_ALLOW, CONTENT_SETTING_BLOCK},
           WebsiteSettingsInfo::TOP_ORIGIN_ONLY_SCOPE,
           WebsiteSettingsRegistry::DESKTOP |
               WebsiteSettingsRegistry::PLATFORM_ANDROID,
           ContentSettingsInfo::INHERIT_IN_INCOGNITO,
           ContentSettingsInfo::EXCEPTIONS_ON_SECURE_AND_INSECURE_ORIGINS);

  Register(ContentSettingsType::HNS_REFERRERS, hns_shields::kReferrers,
           CONTENT_SETTING_BLOCK, WebsiteSettingsInfo::SYNCABLE,
           /*allowlisted_schemes=*/{},
           /*valid_settings=*/{CONTENT_SETTING_ALLOW, CONTENT_SETTING_BLOCK},
           WebsiteSettingsInfo::TOP_ORIGIN_ONLY_SCOPE,
           WebsiteSettingsRegistry::DESKTOP |
               WebsiteSettingsRegistry::PLATFORM_ANDROID,
           ContentSettingsInfo::INHERIT_IN_INCOGNITO,
           ContentSettingsInfo::EXCEPTIONS_ON_SECURE_AND_INSECURE_ORIGINS);

  Register(
      ContentSettingsType::HNS_COOKIES, hns_shields::kCookies,
      CONTENT_SETTING_DEFAULT, WebsiteSettingsInfo::SYNCABLE,
      /*allowlisted_schemes=*/{kChromeUIScheme, kChromeDevToolsScheme},
      /*valid_settings=*/{CONTENT_SETTING_ALLOW, CONTENT_SETTING_BLOCK},
      WebsiteSettingsInfo::REQUESTING_ORIGIN_WITH_TOP_ORIGIN_EXCEPTIONS_SCOPE,
      WebsiteSettingsRegistry::DESKTOP |
          WebsiteSettingsRegistry::PLATFORM_ANDROID,
      ContentSettingsInfo::INHERIT_IN_INCOGNITO,
      ContentSettingsInfo::EXCEPTIONS_ON_SECURE_AND_INSECURE_ORIGINS);

  Register(
      ContentSettingsType::HNS_COSMETIC_FILTERING,
      hns_shields::kCosmeticFiltering, CONTENT_SETTING_DEFAULT,
      WebsiteSettingsInfo::SYNCABLE, /*allowlisted_schemes=*/{},
      /*valid_settings=*/{CONTENT_SETTING_ALLOW, CONTENT_SETTING_BLOCK},
      WebsiteSettingsInfo::REQUESTING_ORIGIN_WITH_TOP_ORIGIN_EXCEPTIONS_SCOPE,
      WebsiteSettingsRegistry::DESKTOP |
          WebsiteSettingsRegistry::PLATFORM_ANDROID,
      ContentSettingsInfo::INHERIT_IN_INCOGNITO,
      ContentSettingsInfo::EXCEPTIONS_ON_SECURE_AND_INSECURE_ORIGINS);

  Register(ContentSettingsType::HNS_FINGERPRINTING_V2,
           hns_shields::kFingerprintingV2, CONTENT_SETTING_ASK,
           WebsiteSettingsInfo::SYNCABLE, /*allowlisted_schemes=*/{},
           /*valid_settings=*/
           {CONTENT_SETTING_ALLOW, CONTENT_SETTING_BLOCK, CONTENT_SETTING_ASK},
           WebsiteSettingsInfo::TOP_ORIGIN_ONLY_SCOPE,
           WebsiteSettingsRegistry::DESKTOP |
               WebsiteSettingsRegistry::PLATFORM_ANDROID,
           ContentSettingsInfo::INHERIT_IN_INCOGNITO,
           ContentSettingsInfo::EXCEPTIONS_ON_SECURE_AND_INSECURE_ORIGINS);

  Register(ContentSettingsType::HNS_SHIELDS, hns_shields::kHnsShields,
           CONTENT_SETTING_ALLOW, WebsiteSettingsInfo::SYNCABLE,
           /*allowlisted_schemes=*/{},
           /*valid_settings=*/{CONTENT_SETTING_ALLOW, CONTENT_SETTING_BLOCK},
           WebsiteSettingsInfo::TOP_ORIGIN_ONLY_SCOPE,
           WebsiteSettingsRegistry::DESKTOP |
               WebsiteSettingsRegistry::PLATFORM_ANDROID,
           ContentSettingsInfo::INHERIT_IN_INCOGNITO,
           ContentSettingsInfo::EXCEPTIONS_ON_SECURE_AND_INSECURE_ORIGINS);

  Register(ContentSettingsType::HNS_SPEEDREADER, "hnsSpeedreader",
           CONTENT_SETTING_DEFAULT, WebsiteSettingsInfo::SYNCABLE,
           /*allowlisted_schemes=*/{},
           /*valid_settings=*/{CONTENT_SETTING_ALLOW, CONTENT_SETTING_BLOCK},
           WebsiteSettingsInfo::TOP_ORIGIN_ONLY_SCOPE,
           WebsiteSettingsRegistry::DESKTOP |
               WebsiteSettingsRegistry::PLATFORM_ANDROID,
           ContentSettingsInfo::INHERIT_IN_INCOGNITO,
           ContentSettingsInfo::EXCEPTIONS_ON_SECURE_AND_INSECURE_ORIGINS);

  // Add CONTENT_SETTING_ASK for autoplay
  // Note ASK has been deprecated, only keeping it for
  // DiscardObsoleteAutoplayAsk test case
  content_settings_info_.erase(ContentSettingsType::AUTOPLAY);
  website_settings_registry_->UnRegister(ContentSettingsType::AUTOPLAY);
  Register(ContentSettingsType::AUTOPLAY, "autoplay", CONTENT_SETTING_ALLOW,
           WebsiteSettingsInfo::UNSYNCABLE, /*allowlisted_schemes=*/{},
           /*valid_settings=*/
           {CONTENT_SETTING_ALLOW, CONTENT_SETTING_BLOCK, CONTENT_SETTING_ASK},
           WebsiteSettingsInfo::TOP_ORIGIN_ONLY_SCOPE,
           WebsiteSettingsRegistry::DESKTOP |
               WebsiteSettingsRegistry::PLATFORM_ANDROID,
           ContentSettingsInfo::INHERIT_IN_INCOGNITO,
           ContentSettingsInfo::EXCEPTIONS_ON_SECURE_AND_INSECURE_ORIGINS);

  // Register ethereum default value as Ask.
  Register(ContentSettingsType::HNS_ETHEREUM, "hns_ethereum",
           CONTENT_SETTING_ASK, WebsiteSettingsInfo::UNSYNCABLE,
           /*allowlisted_schemes=*/{},
           /*valid_settings=*/
           {CONTENT_SETTING_ALLOW, CONTENT_SETTING_BLOCK, CONTENT_SETTING_ASK},
           WebsiteSettingsInfo::TOP_ORIGIN_ONLY_SCOPE,
           WebsiteSettingsRegistry::DESKTOP |
               WebsiteSettingsRegistry::PLATFORM_ANDROID,
           ContentSettingsInfo::INHERIT_IF_LESS_PERMISSIVE,
           ContentSettingsInfo::EXCEPTIONS_ON_SECURE_AND_INSECURE_ORIGINS);

  // Register solana default value as Ask.
  Register(ContentSettingsType::HNS_SOLANA, "hns_solana",
           CONTENT_SETTING_ASK, WebsiteSettingsInfo::UNSYNCABLE,
           /*allowlisted_schemes=*/{},
           /*valid_settings=*/
           {CONTENT_SETTING_ALLOW, CONTENT_SETTING_BLOCK, CONTENT_SETTING_ASK},
           WebsiteSettingsInfo::TOP_ORIGIN_ONLY_SCOPE,
           WebsiteSettingsRegistry::DESKTOP |
               WebsiteSettingsRegistry::PLATFORM_ANDROID,
           ContentSettingsInfo::INHERIT_IF_LESS_PERMISSIVE,
           ContentSettingsInfo::EXCEPTIONS_ON_SECURE_AND_INSECURE_ORIGINS);

  // Register google sign in social media permission default value as Ask.
  // This is INHERIT_IN_INCOGNITO because it sets cookie rules, and cookies
  // are INHERIT_IN_INCOGNITO.
  // See https://github.com/hnsvn/hns-core/pull/15330#discussion_r1049643580
  Register(ContentSettingsType::HNS_GOOGLE_SIGN_IN, "hns_google_sign_in",
           CONTENT_SETTING_ASK, WebsiteSettingsInfo::UNSYNCABLE,
           /*allowlisted_schemes=*/{},
           /*valid_settings=*/
           {CONTENT_SETTING_ALLOW, CONTENT_SETTING_BLOCK, CONTENT_SETTING_ASK},
           WebsiteSettingsInfo::TOP_ORIGIN_ONLY_SCOPE,
           WebsiteSettingsRegistry::DESKTOP |
               WebsiteSettingsRegistry::PLATFORM_ANDROID,
           ContentSettingsInfo::INHERIT_IN_INCOGNITO,
           ContentSettingsInfo::EXCEPTIONS_ON_SECURE_AND_INSECURE_ORIGINS);

  Register(ContentSettingsType::HNS_REMEMBER_1P_STORAGE,
           "hns_remember_1p_storage",
           net::features::kHnsForgetFirstPartyStorageByDefault.Get()
               ? CONTENT_SETTING_BLOCK
               : CONTENT_SETTING_ALLOW,
           WebsiteSettingsInfo::UNSYNCABLE, {},
           {CONTENT_SETTING_ALLOW, CONTENT_SETTING_BLOCK},
           WebsiteSettingsInfo::TOP_ORIGIN_ONLY_SCOPE,
           WebsiteSettingsRegistry::DESKTOP |
               WebsiteSettingsRegistry::PLATFORM_ANDROID,
           ContentSettingsInfo::INHERIT_IN_INCOGNITO,
           ContentSettingsInfo::EXCEPTIONS_ON_SECURE_AND_INSECURE_ORIGINS);

  // Register localhost permission default value as Ask.
  Register(ContentSettingsType::HNS_LOCALHOST_ACCESS,
           "hns_localhost_access", CONTENT_SETTING_ASK,
           WebsiteSettingsInfo::UNSYNCABLE,
           /*allowlisted_schemes=*/{},
           /*valid_settings=*/
           {CONTENT_SETTING_ALLOW, CONTENT_SETTING_BLOCK, CONTENT_SETTING_ASK},
           WebsiteSettingsInfo::TOP_ORIGIN_ONLY_SCOPE,
           WebsiteSettingsRegistry::DESKTOP |
               WebsiteSettingsRegistry::PLATFORM_ANDROID,
           ContentSettingsInfo::INHERIT_IF_LESS_PERMISSIVE,
           ContentSettingsInfo::EXCEPTIONS_ON_SECURE_AND_INSECURE_ORIGINS);

  // Disable background sync by default (hns/hns-browser#4709)
  content_settings_info_.erase(ContentSettingsType::BACKGROUND_SYNC);
  website_settings_registry_->UnRegister(ContentSettingsType::BACKGROUND_SYNC);
  Register(ContentSettingsType::BACKGROUND_SYNC, "background-sync",
           CONTENT_SETTING_BLOCK, WebsiteSettingsInfo::UNSYNCABLE,
           /*allowlisted_schemes=*/{},
           /*valid_settings=*/{CONTENT_SETTING_ALLOW, CONTENT_SETTING_BLOCK},
           WebsiteSettingsInfo::TOP_ORIGIN_ONLY_SCOPE,
           WebsiteSettingsRegistry::DESKTOP |
               WebsiteSettingsRegistry::PLATFORM_ANDROID,
           ContentSettingsInfo::INHERIT_IN_INCOGNITO,
           ContentSettingsInfo::EXCEPTIONS_ON_SECURE_ORIGINS_ONLY);

  // Disable motion sensors by default (hns/hns-browser#4789)
  content_settings_info_.erase(ContentSettingsType::SENSORS);
  website_settings_registry_->UnRegister(ContentSettingsType::SENSORS);
  Register(ContentSettingsType::SENSORS, "sensors", CONTENT_SETTING_BLOCK,
           WebsiteSettingsInfo::UNSYNCABLE, /*allowlisted_schemes=*/{},
           /*valid_settings=*/{CONTENT_SETTING_ALLOW, CONTENT_SETTING_BLOCK},
           WebsiteSettingsInfo::TOP_ORIGIN_ONLY_SCOPE,
           WebsiteSettingsRegistry::DESKTOP |
               WebsiteSettingsRegistry::PLATFORM_ANDROID,
           ContentSettingsInfo::INHERIT_IN_INCOGNITO,
           ContentSettingsInfo::EXCEPTIONS_ON_SECURE_AND_INSECURE_ORIGINS);

  website_settings_registry_->UnRegister(ContentSettingsType::HTTP_ALLOWED);
  website_settings_registry_->Register(
      ContentSettingsType::HTTP_ALLOWED, "http-allowed", base::Value(),
      WebsiteSettingsInfo::UNSYNCABLE, WebsiteSettingsInfo::NOT_LOSSY,
      WebsiteSettingsInfo::GENERIC_SINGLE_ORIGIN_SCOPE,
      WebsiteSettingsRegistry::DESKTOP |
          WebsiteSettingsRegistry::PLATFORM_ANDROID,
      WebsiteSettingsInfo::DONT_INHERIT_IN_INCOGNITO);
}

}  // namespace content_settings
