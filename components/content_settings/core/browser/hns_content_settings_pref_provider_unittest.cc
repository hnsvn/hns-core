/*  Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include <memory>
#include <utility>

#include "base/json/values_util.h"
#include "base/memory/raw_ptr.h"
#include "base/values.h"
#include "hns/components/hns_shields/common/hns_shield_constants.h"
#include "hns/components/constants/pref_names.h"
#include "hns/components/content_settings/core/browser/hns_content_settings_pref_provider.h"
#include "hns/components/content_settings/core/browser/hns_content_settings_utils.h"
#include "chrome/test/base/testing_profile.h"
#include "components/content_settings/core/browser/content_settings_pref.h"
#include "components/content_settings/core/browser/content_settings_registry.h"
#include "components/content_settings/core/common/content_settings.h"
#include "components/content_settings/core/common/content_settings_pattern.h"
#include "components/content_settings/core/common/content_settings_types.h"
#include "components/content_settings/core/common/content_settings_utils.h"
#include "components/content_settings/core/test/content_settings_test_utils.h"
#include "components/prefs/pref_service.h"
#include "components/prefs/scoped_user_pref_update.h"
#include "content/public/test/browser_task_environment.h"
#include "testing/gtest/include/gtest/gtest.h"
#include "third_party/abseil-cpp/absl/types/optional.h"
#include "url/gurl.h"

namespace content_settings {

namespace {

constexpr char kUserProfilePluginsPath[] =
    "profile.content_settings.exceptions.plugins";

const char kExpirationPath[] = "expiration";
const char kLastModifiedPath[] = "last_modified";
const char kSessionModelPath[] = "model";
const char kSettingPath[] = "setting";
const char kPerResourcePath[] = "per_resource";

using GURLSourcePair = std::pair<GURL, ContentSettingsType>;

ContentSettingsPattern SecondaryUrlToPattern(const GURL& gurl) {
  CHECK(gurl == GURL() || gurl == GURL("https://firstParty/*"));
  if (gurl == GURL())
    return ContentSettingsPattern::Wildcard();
  else
    return ContentSettingsPattern::FromString("https://firstParty/*");
}

base::Value::Dict* InitializeCommonSettingsAndGetPerResourceDictionary(
    base::Value::Dict* dict,
    const base::Time& last_modified_time) {
  const uint64_t last_modified_time_in_ms =
      last_modified_time.ToDeltaSinceWindowsEpoch().InMicroseconds();

  dict->SetByDottedPath(kExpirationPath, 0);
  dict->SetByDottedPath(kLastModifiedPath,
                        base::NumberToString(last_modified_time_in_ms));
  dict->SetByDottedPath(
      kSessionModelPath,
      static_cast<int>(content_settings::SessionModel::Durable));

  return dict->EnsureDict(kPerResourcePath);
}

void InitializeAllShieldSettingsInDictionary(
    base::Value::Dict* dict,
    const base::Time& last_modified_time,
    int value) {
  base::Value::Dict* per_resource_dict =
      InitializeCommonSettingsAndGetPerResourceDictionary(dict,
                                                          last_modified_time);
  per_resource_dict->Set(hns_shields::kAds, value);
  per_resource_dict->Set(hns_shields::kCookies, value);
  per_resource_dict->Set(hns_shields::kCosmeticFiltering, value);
  per_resource_dict->Set(hns_shields::kFingerprintingV2, value);
  per_resource_dict->Set(hns_shields::kHTTPUpgradableResources, value);
  per_resource_dict->Set(hns_shields::kReferrers, value);
  per_resource_dict->Set(hns_shields::kTrackers, value);
}

void InitializeHnsShieldsSettingInDictionary(
    base::Value::Dict* dict,
    const base::Time& last_modified_time,
    int value) {
  base::Value::Dict* per_resource_dict =
      InitializeCommonSettingsAndGetPerResourceDictionary(dict,
                                                          last_modified_time);
  per_resource_dict->Set(hns_shields::kHnsShields, value);
}

void InitializeUnsupportedShieldSettingInDictionary(
    base::Value::Dict* dict,
    const base::Time& last_modified_time) {
  base::Value::Dict* per_resource_dict =
      InitializeCommonSettingsAndGetPerResourceDictionary(dict,
                                                          last_modified_time);
  per_resource_dict->Set("unknown_setting", 1);
}

void CheckMigrationFromResourceIdentifierForDictionary(
    const base::Value::Dict& dict,
    base::StringPiece patterns_string,
    const absl::optional<base::Time> expected_last_modified,
    absl::optional<int> expected_setting_value) {
  const base::Value::Dict* settings_dict = dict.FindDict(patterns_string);
  EXPECT_NE(settings_dict, nullptr);

  auto actual_value = settings_dict->FindInt(kSettingPath);
  EXPECT_EQ(base::ValueToTime(settings_dict->Find(kLastModifiedPath)),
            expected_last_modified);
  EXPECT_EQ(GetSessionModelFromDictionary(*settings_dict, kSessionModelPath),
            content_settings::SessionModel::Durable);
  EXPECT_EQ(actual_value, expected_setting_value);
}

class ShieldsSetting {
 public:
  ShieldsSetting(HnsPrefProvider* provider,
                 const std::vector<GURLSourcePair> urls)
      : provider_(provider), urls_(urls) {}
  virtual ~ShieldsSetting() = default;

  virtual void SetPreMigrationSettings(const ContentSettingsPattern& pattern,
                                       ContentSetting setting) {
    for (const auto& url_source : urls_) {
      provider_->SetWebsiteSetting(
          pattern, SecondaryUrlToPattern(url_source.first), url_source.second,
          ContentSettingToValue(setting), {});
    }
  }

  void CheckSettingsAreDefault(const GURL& url) const {
    CheckSettings(url, CONTENT_SETTING_DEFAULT);
  }

  void CheckSettingsWouldBlock(const GURL& url) const {
    CheckSettings(url, CONTENT_SETTING_BLOCK);
  }

  void CheckSettingsWouldAllow(const GURL& url) const {
    CheckSettings(url, CONTENT_SETTING_ALLOW);
  }

  void CheckSettingsWouldAsk(const GURL& url) const {
    CheckSettings(url, CONTENT_SETTING_ASK);
  }

 protected:
  virtual void CheckSettings(const GURL& url, ContentSetting setting) const {
    for (const auto& url_source : urls_) {
      EXPECT_EQ(setting,
                TestUtils::GetContentSetting(provider_, url, url_source.first,
                                             url_source.second, false));
    }
  }

  raw_ptr<HnsPrefProvider> provider_ = nullptr;
  const std::vector<GURLSourcePair> urls_;
};

class ShieldsCookieSetting : public ShieldsSetting {
 public:
  ShieldsCookieSetting(HnsPrefProvider* provider, PrefService* prefs)
      : ShieldsSetting(
            provider,
            {{GURL(), ContentSettingsType::HNS_COOKIES},
             {GURL("https://firstParty/*"), ContentSettingsType::HNS_COOKIES},
             {GURL(), ContentSettingsType::HNS_REFERRERS}}),
        prefs_(prefs) {}

  void RollbackShieldsCookiesVersion() {
    base::Value::Dict shieldsCookies =
        prefs_->GetDict("profile.content_settings.exceptions.shieldsCookiesV3")
            .Clone();
    prefs_->Set("profile.content_settings.exceptions.shieldsCookies",
                base::Value(std::move(shieldsCookies)));
    prefs_->ClearPref("profile.content_settings.exceptions.shieldsCookiesV3");
  }

 private:
  void CheckSettings(const GURL& url, ContentSetting setting) const override {
    if (prefs_->GetInteger(kHnsShieldsSettingsVersion) < 3) {
      return ShieldsSetting::CheckSettings(url, setting);
    }
    // We need this because if version below than 3 hns cookies patterns
    // are reversed.
    for (const auto& url_source : urls_) {
      if (url_source.second == ContentSettingsType::HNS_COOKIES) {
        EXPECT_EQ(setting,
                  TestUtils::GetContentSetting(provider_, url_source.first, url,
                                               url_source.second, false));
      } else {
        EXPECT_EQ(setting,
                  TestUtils::GetContentSetting(provider_, url, url_source.first,
                                               url_source.second, false));
      }
    }
  }

  raw_ptr<PrefService> prefs_ = nullptr;
};

class CookieSettings : public ShieldsSetting {
 public:
  explicit CookieSettings(HnsPrefProvider* provider)
      : ShieldsSetting(provider, {}) {}
};

class ShieldsFingerprintingSetting : public ShieldsSetting {
 public:
  explicit ShieldsFingerprintingSetting(HnsPrefProvider* provider)
      : ShieldsSetting(provider, {}) {}

  void SetPreMigrationSettings(const ContentSettingsPattern& pattern,
                               ContentSetting setting) override {
    provider_->SetWebsiteSettingForTest(
        pattern, ContentSettingsPattern::Wildcard(),
        ContentSettingsType::HNS_FINGERPRINTING_V2,
        ContentSettingToValue(setting), {});
  }

  void SetPreMigrationSettingsWithSecondary(
      const ContentSettingsPattern& pattern,
      const ContentSettingsPattern& secondary_pattern,
      ContentSetting setting) {
    provider_->SetWebsiteSettingForTest(
        pattern, secondary_pattern,
        ContentSettingsType::HNS_FINGERPRINTING_V2,
        ContentSettingToValue(setting), {});
  }
};

class ShieldsHTTPSESetting : public ShieldsSetting {
 public:
  explicit ShieldsHTTPSESetting(HnsPrefProvider* provider)
      : ShieldsSetting(
            provider,
            {{GURL(), ContentSettingsType::HNS_HTTP_UPGRADABLE_RESOURCES}}) {}
};

class ShieldsAdsSetting : public ShieldsSetting {
 public:
  explicit ShieldsAdsSetting(HnsPrefProvider* provider)
      : ShieldsSetting(provider,
                       {{GURL(), ContentSettingsType::HNS_ADS},
                        {GURL(), ContentSettingsType::HNS_TRACKERS}}) {}
};

class ShieldsEnabledSetting : public ShieldsSetting {
 public:
  explicit ShieldsEnabledSetting(HnsPrefProvider* provider)
      : ShieldsSetting(provider,
                       {{GURL(), ContentSettingsType::HNS_SHIELDS}}) {}
};

class ShieldsScriptSetting : public ShieldsSetting {
 public:
  explicit ShieldsScriptSetting(HnsPrefProvider* provider)
      : ShieldsSetting(provider, {}) {}

  void SetPreMigrationSettings(const ContentSettingsPattern& pattern,
                               ContentSetting setting) override {
    provider_->SetWebsiteSetting(pattern, ContentSettingsPattern::Wildcard(),
                                 ContentSettingsType::JAVASCRIPT,
                                 ContentSettingToValue(setting), {});
  }

 private:
  void CheckSettings(const GURL& url, ContentSetting setting) const override {
    EXPECT_EQ(setting, TestUtils::GetContentSetting(
                           provider_, url, GURL(),
                           ContentSettingsType::JAVASCRIPT, false));
  }
};

}  // namespace

class HnsPrefProviderTest : public testing::Test {
 public:
  HnsPrefProviderTest() {
    // Ensure all content settings are initialized.
    ContentSettingsRegistry::GetInstance();
  }

  void SetUp() override {
    testing::Test::SetUp();
    testing_profile_ = TestingProfile::Builder().Build();
  }

  void TearDown() override { testing_profile_.reset(); }

  TestingProfile* testing_profile() { return testing_profile_.get(); }

 private:
  content::BrowserTaskEnvironment task_environment_;
  std::unique_ptr<TestingProfile> testing_profile_;
};

TEST_F(HnsPrefProviderTest, TestShieldsSettingsMigration) {
  HnsPrefProvider provider(
      testing_profile()->GetPrefs(), false /* incognito */,
      true /* store_last_modified */, false /* restore_session */);

  ShieldsCookieSetting cookie_settings(&provider,
                                       testing_profile()->GetPrefs());
  ShieldsFingerprintingSetting fp_settings(&provider);
  ShieldsHTTPSESetting httpse_settings(&provider);
  ShieldsAdsSetting ads_settings(&provider);
  ShieldsEnabledSetting enabled_settings(&provider);
  ShieldsScriptSetting script_settings(&provider);

  GURL url("http://hns.com:8080/");
  GURL url2("http://allowed.hns.com:3030");
  // Check that the settings for the url are default values.
  cookie_settings.CheckSettingsAreDefault(url);
  cookie_settings.CheckSettingsAreDefault(url2);
  fp_settings.CheckSettingsAreDefault(url);
  httpse_settings.CheckSettingsAreDefault(url);
  ads_settings.CheckSettingsAreDefault(url);
  enabled_settings.CheckSettingsAreDefault(url);
  script_settings.CheckSettingsAreDefault(url);

  // Set pre-migrtion patterns different from defaults.
  // ------------------------------------------------------
  testing_profile()->GetPrefs()->SetInteger(kHnsShieldsSettingsVersion, 1);

  ContentSettingsPattern pattern = ContentSettingsPattern::FromURL(url);
  ContentSettingsPattern pattern2 = ContentSettingsPattern::FromURL(url2);
  // Cookies.
  cookie_settings.SetPreMigrationSettings(pattern, CONTENT_SETTING_BLOCK);
  cookie_settings.SetPreMigrationSettings(pattern2, CONTENT_SETTING_ALLOW);
  // Pattern that doesn't need to be migrated.
  cookie_settings.SetPreMigrationSettings(
      ContentSettingsPattern::FromString("*://help.hns.com/*"),
      CONTENT_SETTING_BLOCK);
  // Check that settings would block hns.com:8080, but not hns.com:5555.
  cookie_settings.CheckSettingsWouldBlock(url);
  cookie_settings.CheckSettingsWouldAllow(url2);
  cookie_settings.CheckSettingsAreDefault(GURL("http://hns.com:5555"));

  // Finterprinting.
  fp_settings.SetPreMigrationSettings(pattern, CONTENT_SETTING_ALLOW);
  // Check that settings would allow hns.com:8080, but not hns.com:5555.
  fp_settings.CheckSettingsWouldAllow(url);
  fp_settings.CheckSettingsAreDefault(GURL("http://hns.com:5555"));

  // HTTPSE.
  httpse_settings.SetPreMigrationSettings(pattern, CONTENT_SETTING_BLOCK);
  // Check that settings would block hns.com:8080, but not hns.com:5555.
  httpse_settings.CheckSettingsWouldBlock(url);
  httpse_settings.CheckSettingsAreDefault(GURL("http://hns.com:5555"));

  // Ads.
  ads_settings.SetPreMigrationSettings(pattern, CONTENT_SETTING_ALLOW);
  // Check that settings would allow hns.com:8080, but not hns.com:5555.
  ads_settings.CheckSettingsWouldAllow(url);
  ads_settings.CheckSettingsAreDefault(GURL("http://hns.com:5555"));

  // Enabled.
  enabled_settings.SetPreMigrationSettings(pattern, CONTENT_SETTING_BLOCK);
  // Check that settings would block hns.com:8080, but not hns.com:5555.
  httpse_settings.CheckSettingsWouldBlock(url);
  httpse_settings.CheckSettingsAreDefault(GURL("http://hns.com:5555"));

  // Scripts.
  script_settings.SetPreMigrationSettings(pattern, CONTENT_SETTING_BLOCK);
  // Check that settings would block hns.com:8080, but not hns.com:5555.
  script_settings.CheckSettingsWouldBlock(url);
  script_settings.CheckSettingsAreDefault(GURL("http://hns.com:5555"));

  // Migrate settings.
  // ------------------------------------------------------
  cookie_settings.RollbackShieldsCookiesVersion();
  provider.MigrateShieldsSettings(/*incognito*/ false);

  // Check post-migration settings.
  // ------------------------------------------------------
  // Cookies.
  // Check that settings would block hns.com with any protocol and port.
  cookie_settings.CheckSettingsWouldBlock(url);
  cookie_settings.CheckSettingsWouldBlock(GURL("http://hns.com:5555"));
  cookie_settings.CheckSettingsWouldBlock(GURL("https://hns.com"));
  // Check that settings would allow allow.hns.com with any protocol and port.
  cookie_settings.CheckSettingsWouldAllow(url2);
  cookie_settings.CheckSettingsWouldAllow(GURL("https://allowed.hns.com"));
  // Check the pattern that didn't need to be migrated.
  cookie_settings.CheckSettingsWouldBlock(
      GURL("https://help.hns.com/article1.html"));
  // Would not block a different domain.
  cookie_settings.CheckSettingsAreDefault(GURL("http://hns2.com"));

  // Fingerprinting.
  // Check that settings would allow hns.com with any protocol and port.
  fp_settings.CheckSettingsWouldAllow(url);
  fp_settings.CheckSettingsWouldAllow(GURL("http://hns.com:5555"));
  fp_settings.CheckSettingsWouldAllow(GURL("https://hns.com"));
  // Would not allow a different domain.
  fp_settings.CheckSettingsAreDefault(GURL("http://hns2.com"));

  // HTTPSE.
  // Check that settings would block hns.com with any protocol and port.
  httpse_settings.CheckSettingsWouldBlock(url);
  httpse_settings.CheckSettingsWouldBlock(GURL("http://hns.com:5555"));
  // Would not block a different domain.
  httpse_settings.CheckSettingsAreDefault(GURL("http://hns2.com"));

  // Ads.
  // Check that settings would allow hns.com with any protocol and port.
  ads_settings.CheckSettingsWouldAllow(url);
  ads_settings.CheckSettingsWouldAllow(GURL("http://hns.com:5555"));
  ads_settings.CheckSettingsWouldAllow(GURL("https://hns.com"));
  // Would not allow a different domain.
  ads_settings.CheckSettingsAreDefault(GURL("http://hns2.com"));

  // Enabled.
  // Check that settings would block hns.com with any protocol and port.
  httpse_settings.CheckSettingsWouldBlock(url);
  httpse_settings.CheckSettingsWouldBlock(GURL("http://hns.com:5555"));
  httpse_settings.CheckSettingsWouldBlock(GURL("https://hns.com"));
  // Would not block a different domain.
  httpse_settings.CheckSettingsAreDefault(GURL("http://hns2.com"));

  // Scripts.
  // Check that settings would block hns.com with any protocol and port.
  script_settings.CheckSettingsWouldBlock(url);
  script_settings.CheckSettingsWouldBlock(GURL("http://hns.com:5555"));
  script_settings.CheckSettingsWouldBlock(GURL("https://hns.com"));
  // Would not block a different domain.
  script_settings.CheckSettingsAreDefault(GURL("http://hns2.com"));

  provider.ShutdownOnUIThread();
}

TEST_F(HnsPrefProviderTest, TestShieldsSettingsMigrationVersion) {
  PrefService* prefs = testing_profile()->GetPrefs();
  HnsPrefProvider provider(prefs, false /* incognito */,
                             true /* store_last_modified */,
                             false /* restore_session */);

  // Should have migrated when constructed (with profile).
  EXPECT_EQ(4, prefs->GetInteger(kHnsShieldsSettingsVersion));

  // Reset and check that migration runs.
  prefs->SetInteger(kHnsShieldsSettingsVersion, 1);
  provider.MigrateShieldsSettings(/*incognito*/ false);
  EXPECT_EQ(4, prefs->GetInteger(kHnsShieldsSettingsVersion));

  // Test that migration doesn't run for another version.
  prefs->SetInteger(kHnsShieldsSettingsVersion, 5);
  provider.MigrateShieldsSettings(/*incognito*/ false);
  EXPECT_EQ(5, prefs->GetInteger(kHnsShieldsSettingsVersion));

  provider.ShutdownOnUIThread();
}

TEST_F(HnsPrefProviderTest, MigrateFPShieldsSettings) {
  HnsPrefProvider provider(
      testing_profile()->GetPrefs(), false /* incognito */,
      true /* store_last_modified */, false /* restore_session */);

  ShieldsFingerprintingSetting fp_settings(&provider);

  GURL url("http://hns.com:8080/");
  ContentSettingsPattern pattern = ContentSettingsPattern::FromURL(url);
  fp_settings.SetPreMigrationSettings(pattern, CONTENT_SETTING_BLOCK);

  GURL url2("http://hns.com:3030");
  ContentSettingsPattern pattern2 = ContentSettingsPattern::FromURL(url2);
  fp_settings.SetPreMigrationSettingsWithSecondary(
      pattern2, ContentSettingsPattern::FromString("https://balanced/*"),
      CONTENT_SETTING_BLOCK);

  GURL url3("http://hns.com:8181/");
  ContentSettingsPattern pattern3 = ContentSettingsPattern::FromURL(url3);
  fp_settings.SetPreMigrationSettings(pattern3, CONTENT_SETTING_ALLOW);

  GURL url4("http://hns.com:8282/");
  ContentSettingsPattern pattern4 = ContentSettingsPattern::FromURL(url4);
  fp_settings.SetPreMigrationSettings(pattern4, CONTENT_SETTING_ASK);

  provider.MigrateFingerprintingSettings();
  provider.MigrateFingerprintingSetingsToOriginScoped();
#if BUILDFLAG(IS_ANDROID)
  fp_settings.CheckSettingsWouldAsk(url);
#else
  fp_settings.CheckSettingsWouldBlock(url);
#endif
  fp_settings.CheckSettingsWouldAsk(url2);

  // ignore attempts to set balanced settings
  provider.SetWebsiteSetting(
      pattern2, ContentSettingsPattern::FromString("https://balanced/*"),
      ContentSettingsType::HNS_FINGERPRINTING_V2,
      ContentSettingToValue(CONTENT_SETTING_BLOCK), {});
  std::vector<Rule> rules;
  auto rule_iterator = provider.GetRuleIterator(
      ContentSettingsType::HNS_FINGERPRINTING_V2, false);
  while (rule_iterator && rule_iterator->HasNext()) {
    auto rule = rule_iterator->Next();
    EXPECT_NE(
        rule->secondary_pattern.ToString(),
        ContentSettingsPattern::FromString("https://balanced/*").ToString());
  }
  rule_iterator.reset();

  fp_settings.SetPreMigrationSettingsWithSecondary(
      pattern2, ContentSettingsPattern::FromString("https://balanced/*"),
      CONTENT_SETTING_BLOCK);
  // should ignore any balanced setting set after the migration
  fp_settings.CheckSettingsWouldAsk(url2);

  fp_settings.CheckSettingsWouldAllow(url3);
  fp_settings.CheckSettingsWouldAsk(url4);

  provider.ShutdownOnUIThread();
}

TEST_F(HnsPrefProviderTest, TestShieldsSettingsMigrationFromResourceIDs) {
  PrefService* pref_service = testing_profile()->GetPrefs();
  HnsPrefProvider provider(pref_service, false /* incognito */,
                             true /* store_last_modified */,
                             false /* restore_session */);

  // Manually write settings under the PLUGINS type using the no longer existing
  // ResourceIdentifier names, and then perform the migration.
  absl::optional<ScopedDictPrefUpdate> plugins(absl::in_place, pref_service,
                                               kUserProfilePluginsPath);

  base::Time expected_last_modified = base::Time::Now();

  // Seed global shield settings with non-default values.
  base::Value::Dict* global_settings = plugins.value()->EnsureDict("*,*");

  const int expected_global_settings_value = 1;
  InitializeAllShieldSettingsInDictionary(
      global_settings, expected_last_modified, expected_global_settings_value);

  // Change all of those global settings for www.example.com.
  base::Value::Dict* example_settings =
      plugins.value()->EnsureDict("www.example.com,*");

  const int expected_example_com_settings_value = 1;
  InitializeAllShieldSettingsInDictionary(example_settings,
                                          expected_last_modified,
                                          expected_example_com_settings_value);

  // Disable Hns Shields for www.hns.com.
  base::Value::Dict* hns_settings =
      plugins.value()->EnsureDict("www.hns.com,*");

  const int expected_hns_com_settings_value = 1;
  InitializeHnsShieldsSettingInDictionary(hns_settings,
                                            expected_last_modified,
                                            expected_hns_com_settings_value);

  // Destroying `plugins` at this point, as otherwise it will be holding a
  // dangling pointer, after `MigrateShieldsSettingsFromResourceIds()`.
  plugins = absl::nullopt;

  provider.MigrateShieldsSettingsFromResourceIds();

  // Check migration for all the settings has been properly done.
  for (auto content_type : GetShieldsContentSettingsTypes()) {
    const auto& hns_shields_dict =
        pref_service->GetDict(GetShieldsSettingUserPrefsPath(
            GetShieldsContentTypeName(content_type)));

    if (content_type == ContentSettingsType::HNS_SHIELDS) {
      // We only changed the value of HNS_SHIELDS in www.hns.com.
      CheckMigrationFromResourceIdentifierForDictionary(
          hns_shields_dict, "www.hns.com,*", expected_last_modified,
          expected_hns_com_settings_value);
    } else {
      // All the other settings we changed them globally and in www.example.com.
      CheckMigrationFromResourceIdentifierForDictionary(
          hns_shields_dict, "*,*", expected_last_modified,
          expected_global_settings_value);
      CheckMigrationFromResourceIdentifierForDictionary(
          hns_shields_dict, "www.example.com,*", expected_last_modified,
          expected_example_com_settings_value);
    }
  }

  provider.ShutdownOnUIThread();
}

TEST_F(HnsPrefProviderTest, TestShieldsSettingsMigrationFromUnknownSettings) {
  PrefService* pref_service = testing_profile()->GetPrefs();
  HnsPrefProvider provider(pref_service, false /* incognito */,
                             true /* store_last_modified */,
                             false /* restore_session */);

  // Manually write invalid settings under the PLUGINS type using the no longer
  // existing ResourceIdentifier names, to attempt the migration.
  absl::optional<ScopedDictPrefUpdate> plugins(absl::in_place, pref_service,
                                               kUserProfilePluginsPath);

  // Seed both global and per-site shield settings preferences using unsupported
  // names, so that we can test that Hns doesn't crash while attempting the
  // migration and simply ignore those unsupported names instead.
  //
  // For a list of supported names, see |kHnsContentSettingstypes| inside the
  // components/content_settings/core/browser/content_settings_registry.cc
  // override, in the chromium_src/ directory.
  base::Value::Dict* global_settings = plugins.value()->EnsureDict("*,*");
  InitializeUnsupportedShieldSettingInDictionary(global_settings,
                                                 base::Time::Now());
  base::Value::Dict* example_settings =
      plugins.value()->EnsureDict("www.example.com,*");
  InitializeUnsupportedShieldSettingInDictionary(example_settings,
                                                 base::Time::Now());

  // Destroying `plugins` at this point, as otherwise it will be holding a
  // dangling pointer, after `MigrateShieldsSettingsFromResourceIds()`.
  plugins = absl::nullopt;

  // Doing the migration below should NOT get a crash due to invalid settings.
  provider.MigrateShieldsSettingsFromResourceIds();

  // New Shields-specific content settings types should have been created due to
  // the migration, but all should be empty since only invalid data was fed.
  for (auto content_type : GetShieldsContentSettingsTypes()) {
    const auto& hns_shields_dict =
        pref_service->GetDict(GetShieldsSettingUserPrefsPath(
            GetShieldsContentTypeName(content_type)));
    EXPECT_TRUE(hns_shields_dict.empty());
  }

  provider.ShutdownOnUIThread();
}

TEST_F(HnsPrefProviderTest, TestShieldsSettingsMigrationV2toV4) {
  HnsPrefProvider provider(
      testing_profile()->GetPrefs(), false /* incognito */,
      true /* store_last_modified */, false /* restore_session */);

  ShieldsCookieSetting shields_cookie_settings(&provider,
                                               testing_profile()->GetPrefs());
  CookieSettings cookie_settings(&provider);
  ShieldsEnabledSetting shields_enabled_settings(&provider);

  GURL blocked("http://hns.com:8080/");
  GURL allowed("http://allowed.hns.com:3030");

  ContentSettingsPattern allowed_pattern =
      ContentSettingsPattern::FromURL(blocked);
  ContentSettingsPattern blocked_pattern =
      ContentSettingsPattern::FromURL(allowed);
  // ShieldsCookies.
  shields_cookie_settings.SetPreMigrationSettings(allowed_pattern,
                                                  CONTENT_SETTING_BLOCK);
  shields_cookie_settings.SetPreMigrationSettings(blocked_pattern,
                                                  CONTENT_SETTING_ALLOW);

  // Disable shields -> cookies should be allowed.
  shields_enabled_settings.SetPreMigrationSettings(blocked_pattern,
                                                   CONTENT_SETTING_BLOCK);

  shields_cookie_settings.RollbackShieldsCookiesVersion();
  testing_profile()->GetPrefs()->SetInteger(kHnsShieldsSettingsVersion, 2);
  provider.MigrateShieldsSettings(/*incognito*/ false);

  shields_cookie_settings.CheckSettingsWouldAllow(allowed);

  // HNS_COOKIES blocked but COOKIES allowed.
  shields_cookie_settings.CheckSettingsWouldBlock(blocked);
  cookie_settings.CheckSettingsWouldAllow(blocked);

  // Enable shields -> cookies should be blocked according to settings.
  shields_enabled_settings.SetPreMigrationSettings(blocked_pattern,
                                                   CONTENT_SETTING_ALLOW);
  shields_cookie_settings.CheckSettingsWouldBlock(blocked);
  cookie_settings.CheckSettingsWouldBlock(blocked);

  // V3 to V4
  testing_profile()->GetPrefs()->SetInteger(kHnsShieldsSettingsVersion, 3);
  provider.MigrateShieldsSettings(/*incognito*/ false);

  shields_cookie_settings.CheckSettingsWouldBlock(blocked);
  cookie_settings.CheckSettingsWouldBlock(blocked);

  provider.ShutdownOnUIThread();
}

TEST_F(HnsPrefProviderTest, EnsureNoWildcardEntries) {
  HnsPrefProvider provider(
      testing_profile()->GetPrefs(), false /* incognito */,
      true /* store_last_modified */, false /* restore_session */);
  ShieldsEnabledSetting shields_enabled_settings(&provider);
  GURL example_url("https://example.com");
  shields_enabled_settings.CheckSettingsAreDefault(example_url);
  // Set wildcard entry
  auto pattern = ContentSettingsPattern::Wildcard();
  provider.SetWebsiteSetting(pattern, pattern,
                             ContentSettingsType::HNS_SHIELDS,
                             base::Value(CONTENT_SETTING_ALLOW), {});
  // Verify global has changed
  shields_enabled_settings.CheckSettingsWouldAllow(example_url);
  // Remove wildcards
  provider.EnsureNoWildcardEntries(ContentSettingsType::HNS_SHIELDS);
  // Verify global has reset
  shields_enabled_settings.CheckSettingsAreDefault(example_url);

  // Simulate sync updates pref directly.
  base::Value::Dict value;
  value.Set("expiration", "0");
  value.Set("last_modified", "13304670271801570");
  value.Set("model", 0);
  value.Set("setting", 2);

  base::Value::Dict update;
  update.Set("*,*", std::move(value));

  testing_profile()->GetPrefs()->SetDict(
      "profile.content_settings.exceptions.hnsShields", std::move(update));
  base::RunLoop().RunUntilIdle();
  // Verify global has reset
  shields_enabled_settings.CheckSettingsAreDefault(example_url);
  provider.ShutdownOnUIThread();
}

}  //  namespace content_settings
