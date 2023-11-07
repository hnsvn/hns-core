/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "base/containers/contains.h"
#include "base/values.h"
#include "build/build_config.h"
#include "components/content_settings/core/browser/content_settings_info.h"
#include "components/content_settings/core/browser/content_settings_registry.h"
#include "components/content_settings/core/browser/website_settings_info.h"
#include "components/content_settings/core/browser/website_settings_registry.h"
#include "components/content_settings/core/common/content_settings.h"
#include "components/content_settings/core/common/content_settings_types.h"
#include "components/pref_registry/pref_registry_syncable.h"
#include "components/prefs/pref_registry.h"
#include "testing/gmock/include/gmock/gmock.h"
#include "testing/gtest/include/gtest/gtest.h"

// Cannot use an anonymous namespace because WebsiteSettingsRegistry's
// constructor and destructor are private.
namespace content_settings {

using ::testing::Contains;
using ::testing::ElementsAre;

class HnsContentSettingsRegistryTest : public testing::Test {
 protected:
  HnsContentSettingsRegistryTest() = default;

  ContentSettingsRegistry* registry() {
    return ContentSettingsRegistry::GetInstance();
  }
  WebsiteSettingsRegistry* website_settings_registry() {
    return WebsiteSettingsRegistry::GetInstance();
  }
};

TEST_F(HnsContentSettingsRegistryTest, Properties) {
  // The cookies type should be registered.
  const ContentSettingsInfo* info =
      registry()->Get(ContentSettingsType::HNS_COOKIES);
  ASSERT_TRUE(info);

  EXPECT_THAT(info->allowlisted_schemes(), ElementsAre("chrome", "devtools"));

  // Check the other properties are populated correctly.
  EXPECT_FALSE(info->IsSettingValid(CONTENT_SETTING_SESSION_ONLY));
  EXPECT_FALSE(info->IsSettingValid(CONTENT_SETTING_ASK));
  EXPECT_EQ(ContentSettingsInfo::INHERIT_IN_INCOGNITO,
            info->incognito_behavior());

  // Check the WebsiteSettingsInfo is populated correctly.
  const WebsiteSettingsInfo* website_settings_info =
      info->website_settings_info();
  EXPECT_EQ("shieldsCookiesV3", website_settings_info->name());
  EXPECT_EQ("profile.content_settings.exceptions.shieldsCookiesV3",
            website_settings_info->pref_name());
  EXPECT_EQ("profile.default_content_setting_values.shieldsCookiesV3",
            website_settings_info->default_value_pref_name());
  ASSERT_TRUE(website_settings_info->initial_default_value().is_int());
  EXPECT_EQ(CONTENT_SETTING_DEFAULT,
            website_settings_info->initial_default_value().GetInt());
#if BUILDFLAG(IS_ANDROID) || BUILDFLAG(IS_IOS)
  EXPECT_EQ(PrefRegistry::NO_REGISTRATION_FLAGS,
            website_settings_info->GetPrefRegistrationFlags());
#else
  EXPECT_EQ(user_prefs::PrefRegistrySyncable::SYNCABLE_PREF,
            website_settings_info->GetPrefRegistrationFlags());
#endif

  // Check the WebsiteSettingsInfo is registered correctly.
  EXPECT_EQ(
      website_settings_registry()->Get(ContentSettingsType::HNS_COOKIES),
      website_settings_info);
}

// Settings that control access to user data should not be inherited.
// Check that only safe settings are inherited in incognito.
TEST_F(HnsContentSettingsRegistryTest, Inheritance) {
  // These settings are safe to inherit in incognito mode because they only
  // disable features like popup blocking, download blocking or ad blocking.
  // They do not allow access to user data.
  const ContentSettingsType safe_types[] = {
      ContentSettingsType::HNS_ADS,
      ContentSettingsType::HNS_COSMETIC_FILTERING,
      ContentSettingsType::HNS_TRACKERS,
      ContentSettingsType::HNS_HTTP_UPGRADABLE_RESOURCES,
      ContentSettingsType::HNS_FINGERPRINTING_V2,
      ContentSettingsType::HNS_SHIELDS,
      ContentSettingsType::HNS_REFERRERS,
      ContentSettingsType::HNS_COOKIES,
      ContentSettingsType::HNS_SPEEDREADER,
      ContentSettingsType::HNS_GOOGLE_SIGN_IN,
      ContentSettingsType::HNS_HTTPS_UPGRADE,
      ContentSettingsType::HNS_REMEMBER_1P_STORAGE,
  };

  for (const ContentSettingsInfo* info : *registry()) {
    SCOPED_TRACE("Content setting: " + info->website_settings_info()->name());

    if (info->website_settings_info()->type() <
        ContentSettingsType::HNS_START)
      continue;

    if (info->incognito_behavior() ==
        ContentSettingsInfo::INHERIT_IN_INCOGNITO) {
      EXPECT_THAT(safe_types, Contains(info->website_settings_info()->type()));
    }
  }
}

// Check the correct factory default setting is retrieved. Note the factory
// default settings are hard coded, so changing them in ContentSettingsRegistry
// would require this test to be updated.
TEST_F(HnsContentSettingsRegistryTest, GetInitialDefaultSetting) {
  const ContentSettingsInfo* info = nullptr;
  {
    SCOPED_TRACE("Content setting: HNS_ADS");
    info = registry()->Get(ContentSettingsType::HNS_ADS);
    EXPECT_EQ(CONTENT_SETTING_BLOCK, info->GetInitialDefaultSetting());
  }

  {
    SCOPED_TRACE("Content setting: HNS_TRACKERS");
    info = registry()->Get(ContentSettingsType::HNS_TRACKERS);
    EXPECT_EQ(CONTENT_SETTING_BLOCK, info->GetInitialDefaultSetting());
  }

  {
    SCOPED_TRACE("Content setting: HNS_HTTP_UPGRADABLE_RESOURCES");
    info =
        registry()->Get(ContentSettingsType::HNS_HTTP_UPGRADABLE_RESOURCES);
    EXPECT_EQ(CONTENT_SETTING_BLOCK, info->GetInitialDefaultSetting());
  }

  {
    SCOPED_TRACE("Content setting: HNS_SHIELDS");
    info = registry()->Get(ContentSettingsType::HNS_SHIELDS);
    EXPECT_EQ(CONTENT_SETTING_ALLOW, info->GetInitialDefaultSetting());
  }

  {
    SCOPED_TRACE("Content setting: HNS_REFERRERS");
    info = registry()->Get(ContentSettingsType::HNS_REFERRERS);
    EXPECT_EQ(CONTENT_SETTING_BLOCK, info->GetInitialDefaultSetting());
  }

  {
    SCOPED_TRACE("Content setting: HNS_ETHEREUM");
    info = registry()->Get(ContentSettingsType::HNS_ETHEREUM);
    EXPECT_EQ(CONTENT_SETTING_ASK, info->GetInitialDefaultSetting());
  }

  {
    SCOPED_TRACE("Content setting: HNS_SOLANA");
    info = registry()->Get(ContentSettingsType::HNS_SOLANA);
    EXPECT_EQ(CONTENT_SETTING_ASK, info->GetInitialDefaultSetting());
  }

  // TODO(bridiver) - DEFAULT is not a valid setting, but leaving it for
  // now because this requires more extensive changes to fix correctly
  // {
  //   SCOPED_TRACE("Content setting: HNS_FINGERPRINTING_V2");
  //   info = registry()->Get(ContentSettingsType::HNS_FINGERPRINTING_V2);
  //   EXPECT_EQ(CONTENT_SETTING_DEFAULT, info->GetInitialDefaultSetting());
  // }

  // {
  //   SCOPED_TRACE("Content setting: HNS_COSMETIC_FILTERING");
  //   info = registry()->Get(ContentSettingsType::HNS_COSMETIC_FILTERING);
  //   EXPECT_EQ(CONTENT_SETTING_DEFAULT, info->GetInitialDefaultSetting());
  // }

  // {
  //   SCOPED_TRACE("Content setting: HNS_COOKIES");
  //   info = registry()->Get(ContentSettingsType::HNS_COOKIES);
  //   EXPECT_EQ(CONTENT_SETTING_DEFAULT, info->GetInitialDefaultSetting());
  // }

  // {
  //   SCOPED_TRACE("Content setting: HNS_SPEEDREADER");
  //   info = registry()->Get(ContentSettingsType::HNS_SPEEDREADER);
  //   EXPECT_EQ(CONTENT_SETTING_DEFAULT, info->GetInitialDefaultSetting());
  // }
}

}  // namespace content_settings