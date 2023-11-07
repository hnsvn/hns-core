/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/common/url/url_util.h"

#include "testing/gtest/include/gtest/gtest.h"
#include "url/gurl.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads {

TEST(HnsAdsUrlUtilTest, GetUrlWithEmptyQuery) {
  // Arrange

  // Act

  // Assert
  EXPECT_EQ(GURL("https://foo.com/bar"),
            GetUrlWithEmptyQuery(GURL("https://foo.com/bar?baz=qux")));
}

TEST(HnsAdsUrlUtilTest, DoesNotSupportInvalidUrl) {
  // Arrange

  // Act

  // Assert
  EXPECT_FALSE(DoesSupportUrl(GURL("INVALID")));
}

TEST(HnsAdsUrlUtilTest, DoesSupportUrlWithHttpsScheme) {
  // Arrange

  // Act

  // Assert
  EXPECT_TRUE(DoesSupportUrl(GURL("https://foobar.com")));
}

TEST(HnsAdsUrlUtilTest, DoesNotSupportUrlWithHttpScheme) {
  // Arrange

  // Act

  // Assert
  EXPECT_FALSE(DoesSupportUrl(GURL("http://foobar.com")));
}

TEST(HnsAdsUrlUtilTest, DoesNotSupportUrlWithFooBarScheme) {
  // Arrange

  // Act

  // Assert
  EXPECT_FALSE(DoesSupportUrl(GURL("foobar://baz")));
}

TEST(HnsAdsUrlUtilTest, DoesNotSupportHnsSchemeWithFooBarHostName) {
  // Arrange

  // Act

  // Assert
  EXPECT_FALSE(DoesSupportUrl(GURL("hns://foobar")));
}

TEST(HnsAdsUrlUtilTest, DoesSupportHnsSchemeWithWalletHostName) {
  // Arrange

  // Act

  // Assert
  EXPECT_TRUE(DoesSupportUrl(GURL("hns://wallet")));
}

TEST(HnsAdsUrlUtilTest, DoesSupportHnsSchemeWithWalletHostNameAndPath) {
  // Arrange

  // Act

  // Assert
  EXPECT_TRUE(DoesSupportUrl(GURL("hns://wallet/foo")));
}

TEST(HnsAdsUrlUtilTest, DoesSupportHnsSchemeWithSyncHostName) {
  // Arrange

  // Act

  // Assert
  EXPECT_TRUE(DoesSupportUrl(GURL("hns://sync")));
}

TEST(HnsAdsUrlUtilTest, DoesSupportHnsSchemeWithSyncHostNameAndPath) {
  // Arrange

  // Act

  // Assert
  EXPECT_TRUE(DoesSupportUrl(GURL("hns://sync/foo")));
}

TEST(HnsAdsUrlUtilTest, DoesSupportHnsSchemeWithRewardsHostName) {
  // Arrange

  // Act

  // Assert
  EXPECT_TRUE(DoesSupportUrl(GURL("hns://rewards")));
}

TEST(HnsAdsUrlUtilTest, DoesSupportHnsSchemeWithRewardsHostNameAndPath) {
  // Arrange

  // Act

  // Assert
  EXPECT_TRUE(DoesSupportUrl(GURL("hns://rewards/foo")));
}

TEST(HnsAdsUrlUtilTest, DoesNotSupportHnsSchemeWithSettingsHostName) {
  // Arrange

  // Act

  // Assert
  EXPECT_FALSE(DoesSupportUrl(GURL("hns://settings")));
}

TEST(HnsAdsUrlUtilTest,
     DoesNotSupportHnsSchemeWithSettingsHostNameAndFooBarPath) {
  // Arrange

  // Act

  // Assert
  EXPECT_FALSE(DoesSupportUrl(GURL("hns://settings/foobar")));
}

TEST(HnsAdsUrlUtilTest,
     DoesSupportHnsSchemeWithSettingsHostNameAndSearchEnginesPath) {
  // Arrange

  // Act

  // Assert
  EXPECT_TRUE(DoesSupportUrl(GURL("hns://settings/searchEngines")));
}

TEST(
    HnsAdsUrlUtilTest,
    DoesSupportHnsSchemeWithSettingsHostNameSearchEnginesPathAndSearchQuery) {
  // Arrange

  // Act

  // Assert
  EXPECT_TRUE(
      DoesSupportUrl(GURL("hns://settings/searchEngines?search=foobar")));
}

TEST(
    HnsAdsUrlUtilTest,
    DoesNotSupportHnsSchemeWithSettingsHostNameSearchEnginesPathAndMultipleSearchQueries) {
  // Arrange

  // Act

  // Assert
  EXPECT_FALSE(DoesSupportUrl(
      GURL("hns://settings/searchEngines?search=foo&bar=baz")));
}

TEST(
    HnsAdsUrlUtilTest,
    DoesNotSupportHnsSchemeWithSettingsHostNameSearchEnginesPathAndInvalidQuery) {
  // Arrange

  // Act
  // Assert
  EXPECT_FALSE(DoesSupportUrl(GURL("hns://settings/searchEngines?search")));
}

TEST(HnsAdsUrlUtilTest,
     DoesSupportHnsSchemeWithSettingsHostNameAndSearchPath) {
  // Arrange

  // Act

  // Assert
  EXPECT_TRUE(DoesSupportUrl(GURL("hns://settings/search")));
}

TEST(HnsAdsUrlUtilTest,
     DoesSupportHnsSchemeWithSettingsHostNameSearchPathAndSearchQuery) {
  // Arrange

  // Act

  // Assert
  EXPECT_TRUE(DoesSupportUrl(GURL("hns://settings/search?search=foobar")));
}

TEST(
    HnsAdsUrlUtilTest,
    DoesNotSupportHnsSchemeWithSettingsHostNameSearchPathAndMultipleSearchQueries) {
  // Arrange

  // Act

  // Assert
  EXPECT_FALSE(
      DoesSupportUrl(GURL("hns://settings/search?search=foo&bar=baz")));
}

TEST(HnsAdsUrlUtilTest,
     DoesNotSupportHnsSchemeWithSettingsHostNameSearchPathAndInvalidQuery) {
  // Arrange

  // Act

  // Assert
  EXPECT_FALSE(DoesSupportUrl(GURL("hns://settings/search?search")));
}

TEST(HnsAdsUrlUtilTest,
     DoesNotSupportHnsSchemeWithSettingsHostNameAndQuery) {
  // Arrange

  // Act

  // Assert
  EXPECT_FALSE(DoesSupportUrl(GURL("hns://settings/?search=foobar")));
}

TEST(HnsAdsUrlUtilTest,
     DoesNotSupportHnsSchemeWithSettingsHostNameAndInvalidQuery) {
  // Arrange

  // Act

  // Assert
  EXPECT_FALSE(DoesSupportUrl(GURL("hns://settings/?search")));
}

TEST(HnsAdsUrlUtilTest, MalformedUrlIsNotSupported) {
  // Arrange

  // Act

  // Assert
  EXPECT_FALSE(DoesSupportUrl(GURL("http://foobar.com/hns://wallet")));
}

TEST(HnsAdsUrlUtilTest, UrlMatchesPatternWithNoWildcards) {
  // Arrange

  // Act

  // Assert
  EXPECT_TRUE(MatchUrlPattern(GURL("https://www.foo.com/"),
                              /*pattern*/ "https://www.foo.com/"));
}

TEST(HnsAdsUrlUtilTest, UrlWithPathMatchesPatternWithNoWildcards) {
  // Arrange

  // Act

  // Assert
  EXPECT_TRUE(MatchUrlPattern(GURL("https://www.foo.com/bar"),
                              /*pattern*/ "https://www.foo.com/bar"));
}

TEST(HnsAdsUrlUtilTest, UrlDoesNotMatchPattern) {
  // Arrange

  // Act

  // Assert
  EXPECT_FALSE(
      MatchUrlPattern(GURL("https://www.foo.com/"), /*pattern*/ "www.foo.com"));
}

TEST(HnsAdsUrlUtilTest, UrlDoesNotMatchPatternWithMissingEmptyPath) {
  // Arrange

  // Act

  // Assert
  EXPECT_FALSE(MatchUrlPattern(GURL("https://www.foo.com/"),
                               /*pattern*/ "https://www.foo.com"));
}

TEST(HnsAdsUrlUtilTest, UrlMatchesEndWildcardPattern) {
  // Arrange

  // Act

  // Assert
  EXPECT_TRUE(MatchUrlPattern(GURL("https://www.foo.com/bar?key=test"),
                              /*pattern*/ "https://www.foo.com/bar*"));
}

TEST(HnsAdsUrlUtilTest, UrlMatchesMidWildcardPattern) {
  // Arrange

  // Act

  // Assert
  EXPECT_TRUE(MatchUrlPattern(GURL("https://www.foo.com/woo-bar-hoo"),
                              /*pattern*/ "https://www.foo.com/woo*hoo"));
}

TEST(HnsAdsUrlUtilTest, UrlDoesNotMatchMidWildcardPattern) {
  // Arrange

  // Act

  // Assert
  EXPECT_FALSE(MatchUrlPattern(GURL("https://www.foo.com/woo"),
                               /*pattern*/ "https://www.foo.com/woo*hoo"));
}

TEST(HnsAdsUrlUtilTest, SameDomainOrHost) {
  // Arrange

  // Act

  // Assert
  EXPECT_TRUE(SameDomainOrHost(GURL("https://foo.com?bar=test"),
                               GURL("https://subdomain.foo.com/bar")));
}

TEST(HnsAdsUrlUtilTest, NotSameDomainOrHost) {
  // Arrange

  // Act

  // Assert
  EXPECT_FALSE(SameDomainOrHost(GURL("https://foo.com?bar=test"),
                                GURL("https://subdomain.bar.com/foo")));
}

TEST(HnsAdsUrlUtilTest, SameDomainOrHostForUrlWithNoSubdomain) {
  // Arrange

  // Act

  // Assert
  EXPECT_TRUE(SameDomainOrHost(GURL("https://foo.com?bar=test"),
                               GURL("https://foo.com/bar")));
}

TEST(HnsAdsUrlUtilTest, NotSameDomainOrHostForUrlWithNoSubdomain) {
  // Arrange

  // Act

  // Assert
  EXPECT_FALSE(SameDomainOrHost(GURL("https://foo.com?bar=test"),
                                GURL("https://bar.com/foo")));
}

TEST(HnsAdsUrlUtilTest, SameDomainOrHostForUrlWithRef) {
  // Arrange

  // Act

  // Assert
  EXPECT_TRUE(SameDomainOrHost(GURL("https://foo.com?bar=test#ref"),
                               GURL("https://foo.com/bar")));
}

TEST(HnsAdsUrlUtilTest, NotSameDomainOrHostForUrlWithRef) {
  // Arrange

  // Act

  // Assert
  EXPECT_FALSE(SameDomainOrHost(GURL("https://foo.com?bar=test#ref"),
                                GURL("https://bar.com/foo")));
}

TEST(HnsAdsUrlUtilTest, DomainOrHostExists) {
  // Arrange
  const std::vector<GURL> urls = {GURL("https://foo.com"),
                                  GURL("https://bar.com")};

  // Act

  // Assert
  EXPECT_TRUE(DomainOrHostExists(urls, GURL("https://bar.com/foo")));
}

TEST(HnsAdsUrlUtilTest, DomainOrHostDoesNotExist) {
  // Arrange
  const std::vector<GURL> urls = {GURL("https://foo.com"),
                                  GURL("https://bar.com")};

  // Act

  // Assert
  EXPECT_FALSE(DomainOrHostExists(urls, GURL("https://baz.com/qux")));
}

}  // namespace hns_ads
