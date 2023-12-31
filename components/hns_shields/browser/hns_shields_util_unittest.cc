/*  Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include <memory>
#include <utility>

#include "base/files/scoped_temp_dir.h"
#include "hns/browser/profiles/hns_profile_manager.h"
#include "hns/components/hns_shields/browser/hns_shields_p3a.h"
#include "hns/components/hns_shields/browser/hns_shields_util.h"
#include "hns/components/hns_shields/common/hns_shield_constants.h"
#include "hns/components/hns_shields/common/features.h"
#include "hns/components/constants/pref_names.h"
#include "build/build_config.h"
#include "chrome/browser/browser_process.h"
#include "chrome/browser/content_settings/cookie_settings_factory.h"
#include "chrome/browser/content_settings/host_content_settings_map_factory.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/test/base/scoped_testing_local_state.h"
#include "chrome/test/base/testing_browser_process.h"
#include "chrome/test/base/testing_profile.h"
#include "components/content_settings/core/browser/cookie_settings.h"
#include "components/content_settings/core/browser/host_content_settings_map.h"
#include "components/content_settings/core/common/content_settings_pattern.h"
#include "components/content_settings/core/common/content_settings_types.h"
#include "components/content_settings/core/common/pref_names.h"
#include "components/prefs/pref_service.h"
#include "components/sync_preferences/testing_pref_service_syncable.h"
#include "content/public/test/browser_task_environment.h"
#include "content/public/test/test_utils.h"
#include "net/base/features.h"
#include "testing/gtest/include/gtest/gtest.h"
#include "url/gurl.h"

using hns_shields::ControlType;
using hns_shields::ControlTypeFromString;
using hns_shields::ControlTypeToString;
using hns_shields::DomainBlockingType;
using hns_shields::GetPatternFromURL;
using hns_shields::features::kHnsDomainBlock;

class HnsShieldsUtilTest : public testing::Test {
 public:
  HnsShieldsUtilTest() : local_state_(TestingBrowserProcess::GetGlobal()) {}
  HnsShieldsUtilTest(const HnsShieldsUtilTest&) = delete;
  HnsShieldsUtilTest& operator=(const HnsShieldsUtilTest&) = delete;
  ~HnsShieldsUtilTest() override = default;

  void SetUp() override {
    ASSERT_TRUE(temp_dir_.CreateUniqueTempDir());
    TestingBrowserProcess::GetGlobal()->SetProfileManager(
        CreateProfileManagerForTest());
    TestingProfile::Builder builder;
    builder.SetPath(temp_dir_.GetPath());
    profile_ = builder.Build();
    g_browser_process->profile_manager()->InitProfileUserPrefs(profile_.get());
  }

  void TearDown() override {
    profile_.reset();
    TestingBrowserProcess::GetGlobal()->SetProfileManager(nullptr);
    content::RunAllTasksUntilIdle();
  }

  TestingProfile* profile() { return profile_.get(); }

  void ExpectDomainBlockingType(const GURL& url,
                                DomainBlockingType domain_blocking_type) {
    auto* map = HostContentSettingsMapFactory::GetForProfile(profile());
    auto setting = hns_shields::GetDomainBlockingType(map, url);
    EXPECT_EQ(domain_blocking_type, setting);
  }

 private:
  std::unique_ptr<HnsProfileManager> CreateProfileManagerForTest() {
    return std::make_unique<HnsProfileManagerWithoutInit>(
        temp_dir_.GetPath());
  }

  base::ScopedTempDir temp_dir_;
  content::BrowserTaskEnvironment task_environment_;
  std::unique_ptr<TestingProfile> profile_;
  ScopedTestingLocalState local_state_;
};

class HnsShieldsUtilDomainBlockFeatureTest : public HnsShieldsUtilTest {
 public:
  HnsShieldsUtilDomainBlockFeatureTest() {
    feature_list_.InitAndDisableFeature(kHnsDomainBlock);
  }

 private:
  base::test::ScopedFeatureList feature_list_;
};

TEST_F(HnsShieldsUtilTest, GetPatternFromURL) {
  // wildcard
  auto pattern = GetPatternFromURL(GURL());
  EXPECT_EQ(ContentSettingsPattern::Wildcard(), pattern);

  // scheme is a wildcard, should match any scheme
  pattern = GetPatternFromURL(GURL("http://hns.com"));
  EXPECT_TRUE(pattern.Matches(GURL("http://hns.com")));
  EXPECT_TRUE(pattern.Matches(GURL("http://hns.com/path1")));
  EXPECT_TRUE(pattern.Matches(GURL("http://hns.com/path2")));
  EXPECT_TRUE(pattern.Matches(GURL("https://hns.com")));
  EXPECT_TRUE(pattern.Matches(GURL("ftp://hns.com")));
  EXPECT_FALSE(pattern.Matches(GURL("http://subdomain.hns.com")));
  EXPECT_FALSE(pattern.Matches(GURL("http://hns2.com")));

  // path is a wildcard
  pattern = GetPatternFromURL(GURL("http://hns.com/path1"));
  EXPECT_TRUE(pattern.Matches(GURL("http://hns.com")));
  EXPECT_TRUE(pattern.Matches(GURL("http://hns.com/path1")));
  EXPECT_TRUE(pattern.Matches(GURL("http://hns.com/path2")));
  EXPECT_FALSE(pattern.Matches(GURL("http://subdomain.hns.com")));
  EXPECT_FALSE(pattern.Matches(GURL("http://hns2.com")));

  // port is a wildcard
  pattern = GetPatternFromURL(GURL("http://hns.com:8080"));
  EXPECT_TRUE(pattern.Matches(GURL("http://hns.com")));
  EXPECT_TRUE(pattern.Matches(GURL("http://hns.com:8080")));
  EXPECT_TRUE(pattern.Matches(GURL("http://hns.com:8080/path1")));
  EXPECT_TRUE(pattern.Matches(GURL("http://hns.com:8080/path2")));
  EXPECT_TRUE(pattern.Matches(GURL("http://hns.com:5555")));
  EXPECT_TRUE(pattern.Matches(GURL("https://hns.com")));
  EXPECT_TRUE(pattern.Matches(GURL("https://hns.com:8080")));
  EXPECT_FALSE(pattern.Matches(GURL("http://subdomain.hns.com")));
  EXPECT_FALSE(pattern.Matches(GURL("http://hns2.com")));

  // with implied port
  pattern = GetPatternFromURL(GURL("https://brianbondy.com"));
  EXPECT_EQ(pattern.ToString(), "brianbondy.com");
  pattern = GetPatternFromURL(GURL("http://brianbondy.com"));
  EXPECT_EQ(pattern.ToString(), "brianbondy.com");
  // with specified port
  pattern = GetPatternFromURL(GURL("http://brianbondy.com:8080"));
  EXPECT_EQ(pattern.ToString(), "brianbondy.com");
}

TEST_F(HnsShieldsUtilTest, ControlTypeToString) {
  EXPECT_EQ("block", ControlTypeToString(ControlType::BLOCK));
  EXPECT_EQ("allow", ControlTypeToString(ControlType::ALLOW));
  EXPECT_EQ("block_third_party",
            ControlTypeToString(ControlType::BLOCK_THIRD_PARTY));
}

TEST_F(HnsShieldsUtilTest, ControlTypeFromString) {
  EXPECT_EQ(ControlType::BLOCK, ControlTypeFromString("block"));
  EXPECT_EQ(ControlType::ALLOW, ControlTypeFromString("allow"));
  EXPECT_EQ(ControlType::BLOCK_THIRD_PARTY,
            ControlTypeFromString("block_third_party"));
}

/* HNS_SHIELDS CONTROL */
TEST_F(HnsShieldsUtilTest, SetHnsShieldsEnabled_ForOrigin) {
  auto* map = HostContentSettingsMapFactory::GetForProfile(profile());

  hns_shields::SetHnsShieldsEnabled(map, false, GURL("http://hns.com"));
  // setting should apply to origin
  auto setting = map->GetContentSetting(GURL("http://hns.com"), GURL(),
                                        ContentSettingsType::HNS_SHIELDS);
  EXPECT_EQ(CONTENT_SETTING_BLOCK, setting);

  // setting should apply to different scheme
  setting = map->GetContentSetting(GURL("https://hns.com"), GURL(),
                                   ContentSettingsType::HNS_SHIELDS);
  EXPECT_EQ(CONTENT_SETTING_BLOCK, setting);

  // setting should not apply to default
  setting = map->GetContentSetting(GURL(), GURL(),
                                   ContentSettingsType::HNS_SHIELDS);
  EXPECT_EQ(CONTENT_SETTING_ALLOW, setting);

  GURL host2("http://host2.com");
  GURL host1("http://host1.com");
  // Set shields as disabled for initial value.
  hns_shields::SetHnsShieldsEnabled(map, false, host1);
  // host2.com is enabled by default
  setting =
      map->GetContentSetting(host2, GURL(), ContentSettingsType::HNS_SHIELDS);
  EXPECT_EQ(CONTENT_SETTING_ALLOW, setting);

  // Set policy to disable shields for specific domain.
  base::Value::List disabled_list;
  disabled_list.Append("[*.]host2.com");
  disabled_list.Append("*.*");
  profile()->GetTestingPrefService()->SetManagedPref(
      kManagedHnsShieldsDisabledForUrls, std::move(disabled_list));

  base::Value::List enabled_list;
  enabled_list.Append("[*.]host1.com");
  profile()->GetTestingPrefService()->SetManagedPref(
      kManagedHnsShieldsEnabledForUrls, std::move(enabled_list));

  // setting should apply block to origin.
  setting =
      map->GetContentSetting(host2, GURL(), ContentSettingsType::HNS_SHIELDS);
  EXPECT_EQ(CONTENT_SETTING_BLOCK, setting);
  hns_shields::SetHnsShieldsEnabled(map, true, host2);

  // setting should not be changed.
  EXPECT_FALSE(hns_shields::GetHnsShieldsEnabled(map, host2));

  // setting should apply enabled to origin.
  EXPECT_TRUE(hns_shields::GetHnsShieldsEnabled(map, host1));
  hns_shields::SetHnsShieldsEnabled(map, false, host1);
  // setting should not be changed.
  EXPECT_TRUE(hns_shields::GetHnsShieldsEnabled(map, host1));

  // setting should not apply to default
  EXPECT_TRUE(hns_shields::GetHnsShieldsEnabled(map, GURL()));
}

TEST_F(HnsShieldsUtilTest, IsHnsShieldsManaged) {
  auto* map = HostContentSettingsMapFactory::GetForProfile(profile());
  GURL host2("http://host2.com");
  GURL host1("http://host1.com");
  EXPECT_FALSE(hns_shields::IsHnsShieldsManaged(
      profile()->GetTestingPrefService(), map, host2));

  base::Value::List disabled_list;
  disabled_list.Append("[*.]host2.com");
  profile()->GetTestingPrefService()->SetManagedPref(
      kManagedHnsShieldsDisabledForUrls, std::move(disabled_list));
  // only disabled pref set
  EXPECT_TRUE(hns_shields::IsHnsShieldsManaged(
      profile()->GetTestingPrefService(), map, host2));

  EXPECT_FALSE(hns_shields::IsHnsShieldsManaged(
      profile()->GetTestingPrefService(), map, GURL("http://host1.com")));

  base::Value::List enabled_list;
  enabled_list.Append("[*.]host1.com");
  profile()->GetTestingPrefService()->SetManagedPref(
      kManagedHnsShieldsEnabledForUrls, std::move(enabled_list));

  // both disabled/enabled prefs set
  EXPECT_TRUE(hns_shields::IsHnsShieldsManaged(
      profile()->GetTestingPrefService(), map, host2));

  EXPECT_TRUE(hns_shields::IsHnsShieldsManaged(
      profile()->GetTestingPrefService(), map, host1));

  profile()->GetTestingPrefService()->RemoveManagedPref(
      kManagedHnsShieldsDisabledForUrls);

  // only enabled prefs set
  EXPECT_FALSE(hns_shields::IsHnsShieldsManaged(
      profile()->GetTestingPrefService(), map, host2));

  EXPECT_TRUE(hns_shields::IsHnsShieldsManaged(
      profile()->GetTestingPrefService(), map, host1));
}

TEST_F(HnsShieldsUtilTest, SetHnsShieldsEnabled_IsNotHttpHttps) {
  auto* map = HostContentSettingsMapFactory::GetForProfile(profile());
  auto setting =
      hns_shields::GetHnsShieldsEnabled(map, GURL("chrome://preferences"));
  EXPECT_EQ(false, setting);
  hns_shields::SetHnsShieldsEnabled(map, ControlType::ALLOW,
                                        GURL("chrome://preferences"));
  setting =
      hns_shields::GetHnsShieldsEnabled(map, GURL("chrome://preferences"));
  EXPECT_EQ(false, setting);

  setting = hns_shields::GetHnsShieldsEnabled(map, GURL("about:blank"));
  EXPECT_EQ(false, setting);
  hns_shields::SetHnsShieldsEnabled(map, ControlType::ALLOW,
                                        GURL("about:blank"));
  setting = hns_shields::GetHnsShieldsEnabled(map, GURL("about:blank"));
  EXPECT_EQ(false, setting);
}

TEST_F(HnsShieldsUtilTest, GetHnsShieldsEnabled_ForOrigin) {
  auto* map = HostContentSettingsMapFactory::GetForProfile(profile());

  auto setting = hns_shields::GetHnsShieldsEnabled(map, GURL());
  EXPECT_EQ(true, setting);
  setting =
      hns_shields::GetHnsShieldsEnabled(map, GURL("http://hns.com"));
  EXPECT_EQ(true, setting);
  setting =
      hns_shields::GetHnsShieldsEnabled(map, GURL("https://hns.com"));
  EXPECT_EQ(true, setting);

  /* BLOCK */
  map->SetContentSettingCustomScope(
      ContentSettingsPattern::FromString("http://hns.com/*"),
      ContentSettingsPattern::Wildcard(), ContentSettingsType::HNS_SHIELDS,
      CONTENT_SETTING_BLOCK);
  setting =
      hns_shields::GetHnsShieldsEnabled(map, GURL("http://hns.com/*"));
  EXPECT_EQ(false, setting);
  // https in unchanged
  setting =
      hns_shields::GetHnsShieldsEnabled(map, GURL("https://hns.com"));
  EXPECT_EQ(true, setting);
  // default is unchanged
  setting = hns_shields::GetHnsShieldsEnabled(map, GURL());
  EXPECT_EQ(true, setting);
}

TEST_F(HnsShieldsUtilTest, GetHnsShieldsEnabled_IsNotHttpHttps) {
  auto* map = HostContentSettingsMapFactory::GetForProfile(profile());
  auto setting =
      hns_shields::GetHnsShieldsEnabled(map, GURL("chrome://preferences"));
  EXPECT_EQ(false, setting);

  setting = hns_shields::GetHnsShieldsEnabled(map, GURL("about:blank"));
  EXPECT_EQ(false, setting);
}

/* AD CONTROL */
TEST_F(HnsShieldsUtilTest, SetAdControlType_Default) {
  auto* map = HostContentSettingsMapFactory::GetForProfile(profile());
  // settings should be default
  auto setting =
      map->GetContentSetting(GURL(), GURL(), ContentSettingsType::HNS_ADS);
  EXPECT_EQ(CONTENT_SETTING_BLOCK, setting);
  setting = map->GetContentSetting(GURL("http://hns.com"), GURL(),
                                   ContentSettingsType::HNS_ADS);
  EXPECT_EQ(CONTENT_SETTING_BLOCK, setting);

  /* ALLOW */
  hns_shields::SetAdControlType(map, ControlType::ALLOW, GURL());
  setting =
      map->GetContentSetting(GURL(), GURL(), ContentSettingsType::HNS_ADS);
  EXPECT_EQ(CONTENT_SETTING_ALLOW, setting);

  // override should apply to all origins
  setting = map->GetContentSetting(GURL("http://hns.com"), GURL(),
                                   ContentSettingsType::HNS_ADS);
  EXPECT_EQ(CONTENT_SETTING_ALLOW, setting);

  /* BLOCK */
  hns_shields::SetAdControlType(map, ControlType::BLOCK, GURL());
  setting =
      map->GetContentSetting(GURL(), GURL(), ContentSettingsType::HNS_ADS);
  EXPECT_EQ(CONTENT_SETTING_BLOCK, setting);

  // override should apply to all origins
  setting = map->GetContentSetting(GURL("http://hns.com"), GURL(),
                                   ContentSettingsType::HNS_ADS);
  EXPECT_EQ(CONTENT_SETTING_BLOCK, setting);
}

TEST_F(HnsShieldsUtilTest, SetAdControlType_ForOrigin) {
  auto* map = HostContentSettingsMapFactory::GetForProfile(profile());

  hns_shields::SetAdControlType(map, ControlType::ALLOW,
                                  GURL("http://hns.com"));
  // setting should apply to origin
  auto setting = map->GetContentSetting(GURL("http://hns.com"), GURL(),
                                        ContentSettingsType::HNS_ADS);
  EXPECT_EQ(CONTENT_SETTING_ALLOW, setting);

  // setting should also apply to different scheme
  setting = map->GetContentSetting(GURL("https://hns.com"), GURL(),
                                   ContentSettingsType::HNS_ADS);
  EXPECT_EQ(CONTENT_SETTING_ALLOW, setting);

  // setting should not apply to default
  setting =
      map->GetContentSetting(GURL(), GURL(), ContentSettingsType::HNS_ADS);
  EXPECT_EQ(CONTENT_SETTING_BLOCK, setting);
}

TEST_F(HnsShieldsUtilTest, GetAdControlType_Default) {
  auto* map = HostContentSettingsMapFactory::GetForProfile(profile());

  auto setting = hns_shields::GetAdControlType(map, GURL());
  EXPECT_EQ(ControlType::BLOCK, setting);

  /* ALLOW */
  map->SetContentSettingCustomScope(
      ContentSettingsPattern::Wildcard(), ContentSettingsPattern::Wildcard(),
      ContentSettingsType::HNS_ADS, CONTENT_SETTING_ALLOW);
  setting = hns_shields::GetAdControlType(map, GURL());
  EXPECT_EQ(ControlType::ALLOW, setting);

  /* BLOCK */
  map->SetContentSettingCustomScope(
      ContentSettingsPattern::Wildcard(), ContentSettingsPattern::Wildcard(),
      ContentSettingsType::HNS_ADS, CONTENT_SETTING_BLOCK);
  setting = hns_shields::GetAdControlType(map, GURL());
  EXPECT_EQ(ControlType::BLOCK, setting);
}

TEST_F(HnsShieldsUtilTest, GetAdControlType_ForOrigin) {
  auto* map = HostContentSettingsMapFactory::GetForProfile(profile());

  auto setting = hns_shields::GetAdControlType(map, GURL());
  EXPECT_EQ(ControlType::BLOCK, setting);
  setting = hns_shields::GetAdControlType(map, GURL("http://hns.com"));
  EXPECT_EQ(ControlType::BLOCK, setting);
  setting = hns_shields::GetAdControlType(map, GURL("https://hns.com"));
  EXPECT_EQ(ControlType::BLOCK, setting);

  /* ALLOW */
  map->SetContentSettingCustomScope(
      ContentSettingsPattern::FromString("http://hns.com/*"),
      ContentSettingsPattern::Wildcard(), ContentSettingsType::HNS_ADS,
      CONTENT_SETTING_ALLOW);
  setting = hns_shields::GetAdControlType(map, GURL("http://hns.com"));
  EXPECT_EQ(ControlType::ALLOW, setting);

  // https in unchanged
  setting = hns_shields::GetAdControlType(map, GURL("https://hns.com"));
  EXPECT_EQ(ControlType::BLOCK, setting);
  // default is unchanged
  setting = hns_shields::GetAdControlType(map, GURL());
  EXPECT_EQ(ControlType::BLOCK, setting);

  /* BLOCK */
  // change default to allow
  map->SetContentSettingCustomScope(
      ContentSettingsPattern::Wildcard(), ContentSettingsPattern::Wildcard(),
      ContentSettingsType::HNS_ADS, CONTENT_SETTING_ALLOW);
  setting = hns_shields::GetAdControlType(map, GURL("http://hns.com"));
  EXPECT_EQ(ControlType::ALLOW, setting);
  setting = hns_shields::GetAdControlType(map, GURL("https://hns.com"));
  EXPECT_EQ(ControlType::ALLOW, setting);
  setting = hns_shields::GetAdControlType(map, GURL());
  EXPECT_EQ(ControlType::ALLOW, setting);

  // set override to block
  map->SetContentSettingCustomScope(
      ContentSettingsPattern::FromString("http://hns.com/*"),
      ContentSettingsPattern::Wildcard(), ContentSettingsType::HNS_ADS,
      CONTENT_SETTING_BLOCK);
  setting = hns_shields::GetAdControlType(map, GURL("http://hns.com/*"));
  EXPECT_EQ(ControlType::BLOCK, setting);
  // https in unchanged
  setting = hns_shields::GetAdControlType(map, GURL("https://hns.com"));
  EXPECT_EQ(ControlType::ALLOW, setting);
  // default is unchanged
  setting = hns_shields::GetAdControlType(map, GURL());
  EXPECT_EQ(ControlType::ALLOW, setting);
}

/* COOKIE CONTROL */
TEST_F(HnsShieldsUtilTest, SetCookieControlType_Default) {
  auto* map = HostContentSettingsMapFactory::GetForProfile(profile());
  auto cookies = CookieSettingsFactory::GetForProfile(profile());
  /* ALLOW */
  hns_shields::SetCookieControlType(map, profile()->GetPrefs(),
                                      ControlType::ALLOW, GURL());
  auto setting =
      map->GetContentSetting(GURL(), GURL(), ContentSettingsType::COOKIES);
  EXPECT_EQ(CONTENT_SETTING_ALLOW, setting);
  // setting should apply to all urls
  setting = map->GetContentSetting(GURL("http://hns.com"), GURL(),
                                   ContentSettingsType::COOKIES);
  EXPECT_EQ(CONTENT_SETTING_ALLOW, setting);
  setting =
      map->GetContentSetting(GURL("http://hns.com"), GURL("http://hns.com"),
                             ContentSettingsType::COOKIES);
  EXPECT_EQ(CONTENT_SETTING_ALLOW, setting);

  /* BLOCK */
  hns_shields::SetCookieControlType(map, profile()->GetPrefs(),
                                      ControlType::BLOCK, GURL());
  setting =
      map->GetContentSetting(GURL(), GURL(), ContentSettingsType::COOKIES);
  EXPECT_EQ(CONTENT_SETTING_BLOCK, setting);
  setting = map->GetContentSetting(GURL(), GURL("https://firstParty"),
                                   ContentSettingsType::COOKIES);
  EXPECT_EQ(CONTENT_SETTING_BLOCK, setting);
  EXPECT_FALSE(cookies->ShouldBlockThirdPartyCookies());
  // setting should apply to all urls
  setting = map->GetContentSetting(GURL("http://hns.com"), GURL(),
                                   ContentSettingsType::COOKIES);
  EXPECT_EQ(CONTENT_SETTING_BLOCK, setting);
  setting = map->GetContentSetting(GURL("http://hns.com"),
                                   GURL("https://firstParty"),
                                   ContentSettingsType::COOKIES);
  EXPECT_EQ(CONTENT_SETTING_BLOCK, setting);

  /* BLOCK_THIRD_PARTY */
  hns_shields::SetCookieControlType(map, profile()->GetPrefs(),
                                      ControlType::BLOCK_THIRD_PARTY, GURL());
  setting =
      map->GetContentSetting(GURL(), GURL(), ContentSettingsType::COOKIES);
  EXPECT_EQ(CONTENT_SETTING_ALLOW, setting);
  EXPECT_TRUE(cookies->ShouldBlockThirdPartyCookies());
  setting = map->GetContentSetting(GURL(), GURL("https://firstParty"),
                                   ContentSettingsType::COOKIES);
  EXPECT_EQ(CONTENT_SETTING_ALLOW, setting);

  // setting should apply to all urls
  setting = map->GetContentSetting(GURL("http://hns.com"), GURL(),
                                   ContentSettingsType::COOKIES);
  EXPECT_EQ(CONTENT_SETTING_ALLOW, setting);
  EXPECT_TRUE(cookies->ShouldBlockThirdPartyCookies());
  setting =
      map->GetContentSetting(GURL("http://hns.com"), GURL("http://hns.com"),
                             ContentSettingsType::COOKIES);
  EXPECT_EQ(CONTENT_SETTING_ALLOW, setting);
  EXPECT_TRUE(cookies->ShouldBlockThirdPartyCookies());
}

TEST_F(HnsShieldsUtilTest, SetCookieControlType_ForOrigin) {
  auto* map = HostContentSettingsMapFactory::GetForProfile(profile());
  auto cookies = CookieSettingsFactory::GetForProfile(profile());

  hns_shields::SetCookieControlType(
      map, profile()->GetPrefs(), ControlType::ALLOW, GURL("http://hns.com"));
  // override should apply to origin
  auto setting = hns_shields::GetCookieControlType(map, cookies.get(),
                                                     GURL("http://hns.com"));
  EXPECT_EQ(ControlType::ALLOW, setting);

  // override should also apply to different scheme
  setting = hns_shields::GetCookieControlType(map, cookies.get(),
                                                GURL("http://hns.com"));
  EXPECT_EQ(ControlType::ALLOW, setting);
}

TEST_F(HnsShieldsUtilTest, GetCookieControlType_Default) {
  auto* map = HostContentSettingsMapFactory::GetForProfile(profile());
  auto cookies = CookieSettingsFactory::GetForProfile(profile());

  auto setting =
      hns_shields::GetCookieControlType(map, cookies.get(), GURL());
  EXPECT_EQ(ControlType::BLOCK_THIRD_PARTY, setting);
  setting = hns_shields::GetCookieControlType(map, cookies.get(),
                                                GURL("http://hns.com"));
  EXPECT_EQ(ControlType::BLOCK_THIRD_PARTY, setting);

  /* ALLOW */
  hns_shields::SetCookieControlType(map, profile()->GetPrefs(),
                                      ControlType::ALLOW, GURL());
  setting = hns_shields::GetCookieControlType(map, cookies.get(), GURL());
  EXPECT_EQ(ControlType::ALLOW, setting);
  setting = hns_shields::GetCookieControlType(map, cookies.get(),
                                                GURL("http://hns.com"));
  EXPECT_EQ(ControlType::ALLOW, setting);

  /* BLOCK */
  hns_shields::SetCookieControlType(map, profile()->GetPrefs(),
                                      ControlType::BLOCK, GURL());
  setting = hns_shields::GetCookieControlType(map, cookies.get(), GURL());
  EXPECT_EQ(ControlType::BLOCK, setting);
  setting = hns_shields::GetCookieControlType(map, cookies.get(),
                                                GURL("http://hns.com"));
  EXPECT_EQ(ControlType::BLOCK, setting);

  /* BLOCK_THIRD_PARTY */
  hns_shields::SetCookieControlType(map, profile()->GetPrefs(),
                                      ControlType::BLOCK_THIRD_PARTY, GURL());
  setting = hns_shields::GetCookieControlType(map, cookies.get(), GURL());
  EXPECT_EQ(ControlType::BLOCK_THIRD_PARTY, setting);
  setting = hns_shields::GetCookieControlType(map, cookies.get(),
                                                GURL("http://hns.com"));
  EXPECT_EQ(ControlType::BLOCK_THIRD_PARTY, setting);
}

TEST_F(HnsShieldsUtilTest, GetCookieControlType_WithUserSettings) {
  auto* map = HostContentSettingsMapFactory::GetForProfile(profile());
  auto cookies = CookieSettingsFactory::GetForProfile(profile());

  struct CookieState {
    bool block_first_party = false;
    bool block_third_party = false;
  };

  auto get_effective_cookie_state =
      [](content_settings::CookieSettings* cookie_settings,
         const GURL& url) -> CookieState {
    const auto first_party_blocked =
        cookie_settings->GetCookieSetting(url, url,
                                          net::CookieSettingOverrides(),
                                          nullptr) == CONTENT_SETTING_BLOCK;

    const auto third_party_blocked =
        cookie_settings->GetCookieSetting(GURL::EmptyGURL(), url,
                                          net::CookieSettingOverrides(),
                                          nullptr) == CONTENT_SETTING_BLOCK;

    return {first_party_blocked, third_party_blocked};
  };

  auto cookies_settings =
      get_effective_cookie_state(cookies.get(), GURL("http://hns.com"));
  EXPECT_FALSE(cookies_settings.block_first_party);
  EXPECT_TRUE(cookies_settings.block_third_party);

  // block all
  map->SetContentSettingCustomScope(
      ContentSettingsPattern::Wildcard(),
      ContentSettingsPattern::FromURL(GURL("http://hns.com")),
      ContentSettingsType::COOKIES, CONTENT_SETTING_BLOCK);

  auto setting = hns_shields::GetCookieControlType(map, cookies.get(),
                                                     GURL("http://hns.com"));
  // User settings doesn't affect HNS_COOKIES
  EXPECT_EQ(ControlType::BLOCK_THIRD_PARTY, setting);

  cookies_settings =
      get_effective_cookie_state(cookies.get(), GURL("http://hns.com"));
  EXPECT_TRUE(cookies_settings.block_first_party);
  EXPECT_TRUE(cookies_settings.block_third_party);

  hns_shields::SetCookieControlType(
      map, profile()->GetPrefs(), ControlType::ALLOW, GURL("http://hns.com"));
  setting = hns_shields::GetCookieControlType(map, cookies.get(),
                                                GURL("http://hns.com"));
  // User settings still doesn't affect HNS_COOKIES
  EXPECT_EQ(ControlType::ALLOW, setting);

  cookies_settings =
      get_effective_cookie_state(cookies.get(), GURL("http://hns.com"));
  EXPECT_FALSE(cookies_settings.block_first_party);
  EXPECT_FALSE(cookies_settings.block_third_party);
}

TEST_F(HnsShieldsUtilTest, GetCookieControlType_ForOrigin) {
  auto* map = HostContentSettingsMapFactory::GetForProfile(profile());
  auto cookies = CookieSettingsFactory::GetForProfile(profile());

  auto setting = hns_shields::GetCookieControlType(map, cookies.get(),
                                                     GURL("http://hns.com"));
  EXPECT_EQ(ControlType::BLOCK_THIRD_PARTY, setting);

  /* ALLOW */
  hns_shields::SetCookieControlType(
      map, profile()->GetPrefs(), ControlType::ALLOW, GURL("http://hns.com"));
  setting = hns_shields::GetCookieControlType(map, cookies.get(),
                                                GURL("http://hns.com"));
  EXPECT_EQ(ControlType::ALLOW, setting);
  setting = hns_shields::GetCookieControlType(map, cookies.get(), GURL());
  EXPECT_EQ(ControlType::BLOCK_THIRD_PARTY, setting);

  /* BLOCK */
  hns_shields::SetCookieControlType(
      map, profile()->GetPrefs(), ControlType::BLOCK, GURL("http://hns.com"));
  setting = hns_shields::GetCookieControlType(map, cookies.get(),
                                                GURL("http://hns.com"));
  EXPECT_EQ(ControlType::BLOCK, setting);
  setting = hns_shields::GetCookieControlType(map, cookies.get(), GURL());
  EXPECT_EQ(ControlType::BLOCK_THIRD_PARTY, setting);

  /* BLOCK_THIRD_PARTY */
  hns_shields::SetCookieControlType(map, profile()->GetPrefs(),
                                      ControlType::BLOCK_THIRD_PARTY,
                                      GURL("http://hns.com"));
  setting = hns_shields::GetCookieControlType(map, cookies.get(),
                                                GURL("http://hns.com"));
  EXPECT_EQ(ControlType::BLOCK_THIRD_PARTY, setting);
  setting = hns_shields::GetCookieControlType(map, cookies.get(), GURL());
  EXPECT_EQ(ControlType::BLOCK_THIRD_PARTY, setting);
}

/* FINGERPRINTING CONTROL */
TEST_F(HnsShieldsUtilTest, SetFingerprintingControlType_Default) {
  auto* map = HostContentSettingsMapFactory::GetForProfile(profile());
  // setting should be default to start with
  auto type = hns_shields::GetFingerprintingControlType(map, GURL());
  EXPECT_EQ(ControlType::DEFAULT, type);
  type = hns_shields::GetFingerprintingControlType(map,
                                                     GURL("http://hns.com"));
  EXPECT_EQ(ControlType::DEFAULT, type);

  /* ALLOW */
  hns_shields::SetFingerprintingControlType(map, ControlType::ALLOW, GURL());
  type = hns_shields::GetFingerprintingControlType(map, GURL());
  EXPECT_EQ(ControlType::ALLOW, type);

  // setting should apply to all urls
  type = hns_shields::GetFingerprintingControlType(map,
                                                     GURL("http://hns.com"));
  EXPECT_EQ(ControlType::ALLOW, type);

  /* BLOCK */
  hns_shields::SetFingerprintingControlType(map, ControlType::BLOCK, GURL());
  type = hns_shields::GetFingerprintingControlType(map, GURL());
  EXPECT_EQ(ControlType::BLOCK, type);

  // setting should apply to all urls
  type = hns_shields::GetFingerprintingControlType(map,
                                                     GURL("http://hns.com"));
  EXPECT_EQ(ControlType::BLOCK, type);

  /* DEFAULT */
  hns_shields::SetFingerprintingControlType(map, ControlType::DEFAULT,
                                              GURL());
  type = hns_shields::GetFingerprintingControlType(map, GURL());
  EXPECT_EQ(ControlType::DEFAULT, type);

  // setting should apply to all urls
  type = hns_shields::GetFingerprintingControlType(map,
                                                     GURL("http://hns.com"));
  EXPECT_EQ(ControlType::DEFAULT, type);

  /* Global ALLOW and Site explicit DEFAULT */
  hns_shields::SetFingerprintingControlType(map, ControlType::ALLOW, GURL());
  hns_shields::SetFingerprintingControlType(map, ControlType::DEFAULT,
                                              GURL("http://hns.com"));
  // Site should have DEFAULT if it's explicitly set.
  type = hns_shields::GetFingerprintingControlType(map,
                                                     GURL("http://hns.com"));
  EXPECT_EQ(ControlType::DEFAULT, type);

  /* Global BLOCK and Site explicit DEFAULT */
  hns_shields::SetFingerprintingControlType(map, ControlType::BLOCK, GURL());
  // Site should have DEFAULT if it's explicitly set.
  type = hns_shields::GetFingerprintingControlType(map,
                                                     GURL("http://hns.com"));
  EXPECT_EQ(ControlType::DEFAULT, type);
}

TEST_F(HnsShieldsUtilTest, SetFingerprintingControlType_ForOrigin) {
  auto* map = HostContentSettingsMapFactory::GetForProfile(profile());

  hns_shields::SetFingerprintingControlType(map, ControlType::ALLOW,
                                              GURL("http://hns.com"));
  auto type = hns_shields::GetFingerprintingControlType(
      map, GURL("http://hns.com"));
  EXPECT_EQ(ControlType::ALLOW, type);
  // override should also apply to different scheme
  type = hns_shields::GetFingerprintingControlType(map,
                                                     GURL("https://hns.com"));
  EXPECT_EQ(ControlType::ALLOW, type);

  hns_shields::SetFingerprintingControlType(map, ControlType::BLOCK,
                                              GURL("http://hns.com"));
  type = hns_shields::GetFingerprintingControlType(map,
                                                     GURL("http://hns.com"));
  EXPECT_EQ(ControlType::BLOCK, type);
  // override should also apply to different scheme
  type = hns_shields::GetFingerprintingControlType(map,
                                                     GURL("https://hns.com"));
  EXPECT_EQ(ControlType::BLOCK, type);

  // override should not apply to default
  type = hns_shields::GetFingerprintingControlType(map, GURL());
  EXPECT_EQ(ControlType::DEFAULT, type);
}

/* HTTPSEVERYWHERE CONTROL */
TEST_F(HnsShieldsUtilTest, SetHTTPSEverywhereEnabled_Default) {
  auto* map = HostContentSettingsMapFactory::GetForProfile(profile());
  // settings should be default
  auto setting = map->GetContentSetting(
      GURL(), GURL(), ContentSettingsType::HNS_HTTP_UPGRADABLE_RESOURCES);
  EXPECT_EQ(CONTENT_SETTING_BLOCK, setting);
  setting = map->GetContentSetting(
      GURL("http://hns.com"), GURL(),
      ContentSettingsType::HNS_HTTP_UPGRADABLE_RESOURCES);
  EXPECT_EQ(CONTENT_SETTING_BLOCK, setting);

  /* disabled */
  hns_shields::SetHTTPSEverywhereEnabled(map, false, GURL());
  setting = map->GetContentSetting(
      GURL(), GURL(), ContentSettingsType::HNS_HTTP_UPGRADABLE_RESOURCES);
  EXPECT_EQ(CONTENT_SETTING_ALLOW, setting);

  // override should apply to all origins
  setting = map->GetContentSetting(
      GURL("http://hns.com"), GURL(),
      ContentSettingsType::HNS_HTTP_UPGRADABLE_RESOURCES);
  EXPECT_EQ(CONTENT_SETTING_ALLOW, setting);

  /* enabled */
  hns_shields::SetHTTPSEverywhereEnabled(map, true, GURL());
  setting = map->GetContentSetting(
      GURL(), GURL(), ContentSettingsType::HNS_HTTP_UPGRADABLE_RESOURCES);
  EXPECT_EQ(CONTENT_SETTING_BLOCK, setting);

  // override should apply to all origins
  setting = map->GetContentSetting(
      GURL("http://hns.com"), GURL(),
      ContentSettingsType::HNS_HTTP_UPGRADABLE_RESOURCES);
  EXPECT_EQ(CONTENT_SETTING_BLOCK, setting);
}

TEST_F(HnsShieldsUtilTest, SetHTTPSEverywhereEnabled_ForOrigin) {
  auto* map = HostContentSettingsMapFactory::GetForProfile(profile());

  hns_shields::SetHTTPSEverywhereEnabled(map, false,
                                           GURL("http://hns.com"));
  // setting should apply to origin
  auto setting = map->GetContentSetting(
      GURL("http://hns.com"), GURL(),
      ContentSettingsType::HNS_HTTP_UPGRADABLE_RESOURCES);
  EXPECT_EQ(CONTENT_SETTING_ALLOW, setting);

  // setting should apply to different scheme
  setting = map->GetContentSetting(
      GURL("https://hns.com"), GURL(),
      ContentSettingsType::HNS_HTTP_UPGRADABLE_RESOURCES);
  EXPECT_EQ(CONTENT_SETTING_ALLOW, setting);

  // setting should not apply to default
  setting = map->GetContentSetting(
      GURL(), GURL(), ContentSettingsType::HNS_HTTP_UPGRADABLE_RESOURCES);
  EXPECT_EQ(CONTENT_SETTING_BLOCK, setting);
}

TEST_F(HnsShieldsUtilTest, GetHTTPSEverywhereEnabled_Default) {
  auto* map = HostContentSettingsMapFactory::GetForProfile(profile());

  auto setting = hns_shields::GetHTTPSEverywhereEnabled(map, GURL());
  EXPECT_EQ(true, setting);

  /* ALLOW */
  map->SetContentSettingCustomScope(
      ContentSettingsPattern::Wildcard(), ContentSettingsPattern::Wildcard(),
      ContentSettingsType::HNS_HTTP_UPGRADABLE_RESOURCES,
      CONTENT_SETTING_ALLOW);
  setting = hns_shields::GetHTTPSEverywhereEnabled(map, GURL());
  EXPECT_EQ(false, setting);

  /* BLOCK */
  map->SetContentSettingCustomScope(
      ContentSettingsPattern::Wildcard(), ContentSettingsPattern::Wildcard(),
      ContentSettingsType::HNS_HTTP_UPGRADABLE_RESOURCES,
      CONTENT_SETTING_BLOCK);
  setting = hns_shields::GetHTTPSEverywhereEnabled(map, GURL());
  EXPECT_EQ(true, setting);
}

TEST_F(HnsShieldsUtilTest, GetHTTPSEverywhereEnabled_ForOrigin) {
  auto* map = HostContentSettingsMapFactory::GetForProfile(profile());

  auto setting = hns_shields::GetHTTPSEverywhereEnabled(map, GURL());
  EXPECT_EQ(true, setting);
  setting =
      hns_shields::GetHTTPSEverywhereEnabled(map, GURL("http://hns.com"));
  EXPECT_EQ(true, setting);
  setting =
      hns_shields::GetHTTPSEverywhereEnabled(map, GURL("https://hns.com"));
  EXPECT_EQ(true, setting);

  /* ALLOW */
  map->SetContentSettingCustomScope(
      ContentSettingsPattern::FromString("http://hns.com/*"),
      ContentSettingsPattern::Wildcard(),
      ContentSettingsType::HNS_HTTP_UPGRADABLE_RESOURCES,
      CONTENT_SETTING_ALLOW);
  setting =
      hns_shields::GetHTTPSEverywhereEnabled(map, GURL("http://hns.com"));
  EXPECT_EQ(ControlType::ALLOW, setting);

  // https in unchanged
  setting =
      hns_shields::GetHTTPSEverywhereEnabled(map, GURL("https://hns.com"));
  EXPECT_EQ(true, setting);
  // default is unchanged
  setting = hns_shields::GetHTTPSEverywhereEnabled(map, GURL());
  EXPECT_EQ(true, setting);

  /* BLOCK */
  // change default to allow
  map->SetContentSettingCustomScope(
      ContentSettingsPattern::Wildcard(), ContentSettingsPattern::Wildcard(),
      ContentSettingsType::HNS_HTTP_UPGRADABLE_RESOURCES,
      CONTENT_SETTING_ALLOW);
  setting =
      hns_shields::GetHTTPSEverywhereEnabled(map, GURL("http://hns.com"));
  EXPECT_EQ(ControlType::ALLOW, setting);
  setting =
      hns_shields::GetHTTPSEverywhereEnabled(map, GURL("https://hns.com"));
  EXPECT_EQ(ControlType::ALLOW, setting);
  setting = hns_shields::GetHTTPSEverywhereEnabled(map, GURL());
  EXPECT_EQ(ControlType::ALLOW, setting);

  // set override to block
  map->SetContentSettingCustomScope(
      ContentSettingsPattern::FromString("http://hns.com/*"),
      ContentSettingsPattern::Wildcard(),
      ContentSettingsType::HNS_HTTP_UPGRADABLE_RESOURCES,
      CONTENT_SETTING_BLOCK);
  setting =
      hns_shields::GetHTTPSEverywhereEnabled(map, GURL("http://hns.com/*"));
  EXPECT_EQ(true, setting);
  // https in unchanged
  setting =
      hns_shields::GetHTTPSEverywhereEnabled(map, GURL("https://hns.com"));
  EXPECT_EQ(false, setting);
  // default is unchanged
  setting = hns_shields::GetHTTPSEverywhereEnabled(map, GURL());
  EXPECT_EQ(false, setting);
}

/* NOSCRIPT CONTROL */
TEST_F(HnsShieldsUtilTest, SetNoScriptControlType_Default) {
  auto* map = HostContentSettingsMapFactory::GetForProfile(profile());
  // settings should be default
  auto setting =
      map->GetContentSetting(GURL(), GURL(), ContentSettingsType::JAVASCRIPT);
  EXPECT_EQ(CONTENT_SETTING_ALLOW, setting);
  setting = map->GetContentSetting(GURL("http://hns.com"), GURL(),
                                   ContentSettingsType::JAVASCRIPT);
  EXPECT_EQ(CONTENT_SETTING_ALLOW, setting);

  /* BLOCK */
  hns_shields::SetNoScriptControlType(map, ControlType::BLOCK, GURL());
  setting =
      map->GetContentSetting(GURL(), GURL(), ContentSettingsType::JAVASCRIPT);
  EXPECT_EQ(CONTENT_SETTING_BLOCK, setting);

  // override should apply to all origins
  setting = map->GetContentSetting(GURL("http://hns.com"), GURL(),
                                   ContentSettingsType::JAVASCRIPT);
  EXPECT_EQ(CONTENT_SETTING_BLOCK, setting);

  /* ALLOW */
  hns_shields::SetNoScriptControlType(map, ControlType::ALLOW, GURL());
  setting =
      map->GetContentSetting(GURL(), GURL(), ContentSettingsType::JAVASCRIPT);
  EXPECT_EQ(CONTENT_SETTING_ALLOW, setting);

  // override should apply to all origins
  setting = map->GetContentSetting(GURL("http://hns.com"), GURL(),
                                   ContentSettingsType::JAVASCRIPT);
  EXPECT_EQ(CONTENT_SETTING_ALLOW, setting);
}

TEST_F(HnsShieldsUtilTest, SetNoScriptControlType_ForOrigin) {
  auto* map = HostContentSettingsMapFactory::GetForProfile(profile());

  hns_shields::SetNoScriptControlType(map, ControlType::BLOCK,
                                        GURL("http://hns.com"));
  // setting should apply to origin
  auto setting = map->GetContentSetting(GURL("http://hns.com"), GURL(),
                                        ContentSettingsType::JAVASCRIPT);
  EXPECT_EQ(CONTENT_SETTING_BLOCK, setting);

  // setting should also apply to different scheme
  setting = map->GetContentSetting(GURL("https://hns.com"), GURL(),
                                   ContentSettingsType::JAVASCRIPT);
  EXPECT_EQ(CONTENT_SETTING_BLOCK, setting);

  // setting should not apply to default
  setting =
      map->GetContentSetting(GURL(), GURL(), ContentSettingsType::JAVASCRIPT);
  EXPECT_EQ(CONTENT_SETTING_ALLOW, setting);
}

TEST_F(HnsShieldsUtilTest, GetNoScriptControlType_Default) {
  auto* map = HostContentSettingsMapFactory::GetForProfile(profile());

  auto setting = hns_shields::GetNoScriptControlType(map, GURL());
  EXPECT_EQ(ControlType::ALLOW, setting);

  /* BLOCK */
  map->SetContentSettingCustomScope(
      ContentSettingsPattern::Wildcard(), ContentSettingsPattern::Wildcard(),
      ContentSettingsType::JAVASCRIPT, CONTENT_SETTING_BLOCK);
  setting = hns_shields::GetNoScriptControlType(map, GURL());
  EXPECT_EQ(ControlType::BLOCK, setting);

  /* ALLOW */
  map->SetContentSettingCustomScope(
      ContentSettingsPattern::Wildcard(), ContentSettingsPattern::Wildcard(),
      ContentSettingsType::JAVASCRIPT, CONTENT_SETTING_ALLOW);
  setting = hns_shields::GetNoScriptControlType(map, GURL());
  EXPECT_EQ(ControlType::ALLOW, setting);
}

TEST_F(HnsShieldsUtilTest, GetNoScriptControlType_ForOrigin) {
  auto* map = HostContentSettingsMapFactory::GetForProfile(profile());

  auto setting = hns_shields::GetNoScriptControlType(map, GURL());
  EXPECT_EQ(ControlType::ALLOW, setting);
  setting =
      hns_shields::GetNoScriptControlType(map, GURL("http://hns.com"));
  EXPECT_EQ(ControlType::ALLOW, setting);
  setting =
      hns_shields::GetNoScriptControlType(map, GURL("https://hns.com"));
  EXPECT_EQ(ControlType::ALLOW, setting);

  /* BLOCK */
  // set override to block
  map->SetContentSettingCustomScope(
      ContentSettingsPattern::FromString("http://hns.com/*"),
      ContentSettingsPattern::Wildcard(), ContentSettingsType::JAVASCRIPT,
      CONTENT_SETTING_BLOCK);
  setting =
      hns_shields::GetNoScriptControlType(map, GURL("http://hns.com/*"));
  EXPECT_EQ(ControlType::BLOCK, setting);
  // https in unchanged
  setting =
      hns_shields::GetNoScriptControlType(map, GURL("https://hns.com"));
  EXPECT_EQ(ControlType::ALLOW, setting);
  // default is unchanged
  setting = hns_shields::GetNoScriptControlType(map, GURL());
  EXPECT_EQ(ControlType::ALLOW, setting);

  /* ALLOW */
  // change default to block
  map->SetContentSettingCustomScope(
      ContentSettingsPattern::Wildcard(), ContentSettingsPattern::Wildcard(),
      ContentSettingsType::JAVASCRIPT, CONTENT_SETTING_BLOCK);
  setting =
      hns_shields::GetNoScriptControlType(map, GURL("http://hns.com"));
  EXPECT_EQ(ControlType::BLOCK, setting);
  setting =
      hns_shields::GetNoScriptControlType(map, GURL("https://hns.com"));
  EXPECT_EQ(ControlType::BLOCK, setting);
  setting = hns_shields::GetNoScriptControlType(map, GURL());
  EXPECT_EQ(ControlType::BLOCK, setting);

  map->SetContentSettingCustomScope(
      ContentSettingsPattern::FromString("http://hns.com/*"),
      ContentSettingsPattern::Wildcard(), ContentSettingsType::JAVASCRIPT,
      CONTENT_SETTING_ALLOW);
  setting =
      hns_shields::GetNoScriptControlType(map, GURL("http://hns.com"));
  EXPECT_EQ(ControlType::ALLOW, setting);

  // https in unchanged
  setting =
      hns_shields::GetNoScriptControlType(map, GURL("https://hns.com"));
  EXPECT_EQ(ControlType::BLOCK, setting);
  // default is unchanged
  setting = hns_shields::GetNoScriptControlType(map, GURL());
  EXPECT_EQ(ControlType::BLOCK, setting);
}

// Should not do domain blocking if domain blocking feature is disabled
TEST_F(HnsShieldsUtilDomainBlockFeatureTest, GetDomainBlockingType) {
  ExpectDomainBlockingType(GURL("https://hns.com"),
                           DomainBlockingType::kNone);
}

// Should not do domain blocking if Hns Shields is down
TEST_F(HnsShieldsUtilTest, GetDomainBlockingType_ShieldsDown) {
  auto* map = HostContentSettingsMapFactory::GetForProfile(profile());
  GURL url = GURL("https://hns.com");
  hns_shields::SetHnsShieldsEnabled(map, false, url);
  ExpectDomainBlockingType(url, DomainBlockingType::kNone);
}

// Should not do domain blocking on non-HTTP(S) URLs
TEST_F(HnsShieldsUtilTest, GetDomainBlockingType_IsNotHttpHttps) {
  ExpectDomainBlockingType(GURL("chrome://preferences"),
                           DomainBlockingType::kNone);
  ExpectDomainBlockingType(GURL("about:blank"), DomainBlockingType::kNone);
}

// Should not do domain blocking unless ad blocking is "aggressive"
TEST_F(HnsShieldsUtilTest, GetDomainBlockingType_ControlTypes) {
  auto* map = HostContentSettingsMapFactory::GetForProfile(profile());
  const GURL url = GURL("https://hns.com");

  const struct TestCase {
    ControlType ad_control_type;
    ControlType cosmetic_filtering_control_type;
    DomainBlockingType expected_blocking_type;
  } kTestCases[] = {
      {ControlType::ALLOW, ControlType::ALLOW, DomainBlockingType::kNone},
      {ControlType::ALLOW, ControlType::BLOCK_THIRD_PARTY,
       DomainBlockingType::kNone},
      {ControlType::ALLOW, ControlType::BLOCK, DomainBlockingType::kNone},
      {ControlType::BLOCK, ControlType::ALLOW, DomainBlockingType::kNone},
      {ControlType::BLOCK, ControlType::BLOCK_THIRD_PARTY,
       DomainBlockingType::k1PES},
      {ControlType::BLOCK, ControlType::BLOCK, DomainBlockingType::kAggressive},
  };

  for (const auto& test_case : kTestCases) {
    SCOPED_TRACE(testing::Message()
                 << static_cast<int>(test_case.ad_control_type)
                 << static_cast<int>(test_case.cosmetic_filtering_control_type)
                 << static_cast<int>(test_case.expected_blocking_type));
    hns_shields::SetAdControlType(map, test_case.ad_control_type, url);
    hns_shields::SetCosmeticFilteringControlType(
        map, test_case.cosmetic_filtering_control_type, url);
    ExpectDomainBlockingType(url, test_case.expected_blocking_type);
  }
}

// Should do 1PES domain blocking if domain blocking feature is enabled.
TEST_F(HnsShieldsUtilTest, GetDomainBlockingType) {
  ExpectDomainBlockingType(GURL("https://hns.com"),
                           DomainBlockingType::k1PES);
}
