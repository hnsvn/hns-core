/*  Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include <memory>

#include "hns/components/content_settings/core/browser/hns_content_settings_utils.h"
#include "chrome/test/base/testing_profile.h"
#include "components/content_settings/core/common/content_settings_pattern.h"
#include "content/public/test/browser_task_environment.h"
#include "testing/gtest/include/gtest/gtest.h"
#include "third_party/abseil-cpp/absl/types/optional.h"
#include "url/gurl.h"

using content_settings::ConvertPatternToWildcardSchemeAndPort;

class HnsContentSettingsUtilsTest : public testing::Test {
 public:
  HnsContentSettingsUtilsTest() = default;
  HnsContentSettingsUtilsTest(const HnsContentSettingsUtilsTest&) = delete;
  HnsContentSettingsUtilsTest& operator=(
      const HnsContentSettingsUtilsTest&) = delete;
  ~HnsContentSettingsUtilsTest() override = default;

  void SetUp() override { profile_ = std::make_unique<TestingProfile>(); }

  TestingProfile* profile() { return profile_.get(); }

 private:
  content::BrowserTaskEnvironment task_environment_;
  std::unique_ptr<TestingProfile> profile_;
};

TEST_F(HnsContentSettingsUtilsTest,
       TestConvertPatternToWildcardSchemeAndPort) {
  // Full wildcard pattern.
  EXPECT_EQ(absl::nullopt, ConvertPatternToWildcardSchemeAndPort(
                               ContentSettingsPattern::Wildcard()));

  // Hns first party placeholder pattern.
  EXPECT_EQ(absl::nullopt,
            ConvertPatternToWildcardSchemeAndPort(
                ContentSettingsPattern::FromString("https://firstParty/*")));

  // file:// scheme pattern.
  EXPECT_EQ(absl::nullopt,
            ConvertPatternToWildcardSchemeAndPort(
                ContentSettingsPattern::FromString("file:///a/b/c.zip")));

  // Wildcard host pattern.
  EXPECT_EQ(absl::nullopt,
            ConvertPatternToWildcardSchemeAndPort(
                ContentSettingsPattern::FromString("http://*:8080/*")));

  // Wildcard scheme, no port.
  EXPECT_EQ(absl::nullopt,
            ConvertPatternToWildcardSchemeAndPort(
                ContentSettingsPattern::FromString("*://hns.com/*")));
  EXPECT_EQ(absl::nullopt,
            ConvertPatternToWildcardSchemeAndPort(
                ContentSettingsPattern::FromString("*://hns.com:*/")));

  // Wildcard scheme, has port.
  auto pattern = ConvertPatternToWildcardSchemeAndPort(
      ContentSettingsPattern::FromString("*://hns.com:8080/*"));
  EXPECT_NE(absl::nullopt, pattern);
  EXPECT_EQ(pattern->ToString(), "hns.com");
  EXPECT_TRUE(pattern->Matches(GURL("http://hns.com:80/path1")));
  EXPECT_TRUE(pattern->Matches(GURL("https://hns.com/path2")));
  EXPECT_FALSE(pattern->Matches(GURL("http://hns2.com:8080")));
  pattern.reset();

  // Scheme, no port.
  pattern = ConvertPatternToWildcardSchemeAndPort(
      ContentSettingsPattern::FromString("http://hns.com/"));
  EXPECT_NE(absl::nullopt, pattern);
  EXPECT_EQ(pattern->ToString(), "hns.com");
  EXPECT_TRUE(pattern->Matches(GURL("ftp://hns.com:80/path1")));
  EXPECT_TRUE(pattern->Matches(GURL("https://hns.com/path2")));
  EXPECT_FALSE(pattern->Matches(GURL("http://hns2.com:8080")));
  pattern.reset();

  // Scheme and port.
  pattern = ConvertPatternToWildcardSchemeAndPort(
      ContentSettingsPattern::FromString("https://hns.com:56558/"));
  EXPECT_NE(absl::nullopt, pattern);
  EXPECT_EQ(pattern->ToString(), "hns.com");
  EXPECT_TRUE(pattern->Matches(GURL("wss://hns.com:80/path1")));
  EXPECT_TRUE(pattern->Matches(GURL("https://hns.com/path2")));
  EXPECT_FALSE(pattern->Matches(GURL("http://hns2.com:8080")));
}
