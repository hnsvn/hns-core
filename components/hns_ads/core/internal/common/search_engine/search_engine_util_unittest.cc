/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/common/search_engine/search_engine_util.h"

#include <vector>

#include "base/strings/strcat.h"
#include "hns/components/hns_ads/core/internal/common/search_engine/search_engine_domain_extension_constants.h"
#include "hns/components/hns_ads/core/internal/common/search_engine/search_engine_subdomain_constants.h"
#include "testing/gtest/include/gtest/gtest.h"
#include "url/gurl.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads {

TEST(HnsAdsSearchEngineUtilTest, IsMultilingualAmazonSearchEngine) {
  // Arrange
  const auto domain_extensions = GetAmazonSearchEngineDomainExtensions();

  // Act

  // Assert
  for (const auto& domain_extension : domain_extensions) {
    const GURL url =
        GURL(base::StrCat({"https://www.amazon.", domain_extension, "/"}));
    EXPECT_TRUE(IsSearchEngine(url));
  }
}

TEST(HnsAdsSearchEngineUtilTest, IsNotMultilingualAmazonSearchEngine) {
  // Arrange

  // Act

  // Assert
  EXPECT_FALSE(IsSearchEngine(GURL("https://www.amazon.foobar/")));
}

TEST(HnsAdsSearchEngineUtilTest, IsMultilingualGoogleSearchEngine) {
  // Arrange
  const auto domain_extensions = GetGoogleSearchEngineDomainExtensions();

  // Act

  // Assert
  for (const auto& domain_extension : domain_extensions) {
    const GURL url =
        GURL(base::StrCat({"https://www.google.", domain_extension, "/"}));
    EXPECT_TRUE(IsSearchEngine(url));
  }
}

TEST(HnsAdsSearchEngineUtilTest, IsNotMultilingualGoogleSearchEngine) {
  // Arrange

  // Act

  // Assert
  EXPECT_FALSE(IsSearchEngine(GURL("https://www.google.foobar/")));
}

TEST(HnsAdsSearchEngineUtilTest, IsMultilingualMojeekSearchEngine) {
  // Arrange
  const auto domain_extensions = GetMojeekSearchEngineDomainExtensions();

  // Act

  // Assert
  for (const auto& domain_extension : domain_extensions) {
    const GURL url =
        GURL(base::StrCat({"https://www.mojeek.", domain_extension, "/"}));
    EXPECT_TRUE(IsSearchEngine(url));
  }
}

TEST(HnsAdsSearchEngineUtilTest, IsNotMultilingualMojeekSearchEngine) {
  // Arrange

  // Act

  // Assert
  EXPECT_FALSE(IsSearchEngine(GURL("https://www.mojeek.foobar/")));
}

TEST(HnsAdsSearchEngineUtilTest, IsMultilingualWikipediaSearchEngine) {
  // Arrange
  const auto subdomains = GetWikipediaSearchEngineSubdomains();

  // Act

  // Assert
  for (const auto& subdomain : subdomains) {
    const GURL url =
        GURL(base::StrCat({"https://", subdomain, ".wikipedia.org/"}));
    EXPECT_TRUE(IsSearchEngine(url));
  }
}

TEST(HnsAdsSearchEngineUtilTest, IsNotMultilingualWikipediaSearchEngine) {
  // Arrange

  // Act

  // Assert
  EXPECT_FALSE(IsSearchEngine(GURL("https://foobar.wikipedia.org/")));
}

TEST(HnsAdsSearchEngineUtilTest, IsMultilingualYahooSearchEngine) {
  // Arrange
  const auto subdomains = GetYahooSearchEngineSubdomains();

  // Act

  // Assert
  for (const auto& subdomain : subdomains) {
    const GURL url =
        GURL(base::StrCat({"https://", subdomain, ".search.yahoo.com/"}));
    EXPECT_TRUE(IsSearchEngine(url));
  }
}

TEST(HnsAdsSearchEngineUtilTest, IsNotMultilingualYahooSearchEngine) {
  // Arrange

  // Act

  // Assert
  EXPECT_FALSE(IsSearchEngine(GURL("https://foobar.search.yahoo.com/")));
}

TEST(HnsAdsSearchEngineUtilTest, IsMonolingualSearchEngine) {
  // Arrange
  const std::vector<GURL> urls = {GURL("https://developer.mozilla.org/en-US/"),
                                  GURL("https://duckduckgo.com/"),
                                  GURL("https://fireball.de/"),
                                  GURL("https://github.com/"),
                                  GURL("https://infogalactic.com/"),
                                  GURL("https://search.hns.com/"),
                                  GURL("https://search.yahoo.com/"),
                                  GURL("https://stackoverflow.com/"),
                                  GURL("https://swisscows.com/"),
                                  GURL("https://twitter.com/explore/"),
                                  GURL("https://www.baidu.com/"),
                                  GURL("https://www.bing.com/"),
                                  GURL("https://www.dogpile.com/"),
                                  GURL("https://www.ecosia.org/"),
                                  GURL("https://www.excite.com/"),
                                  GURL("https://www.findx.com/"),
                                  GURL("https://www.gigablast.com/"),
                                  GURL("https://www.lycos.com/"),
                                  GURL("https://www.metacrawler.com/"),
                                  GURL("https://www.petalsearch.com/"),
                                  GURL("https://www.qwant.com/"),
                                  GURL("https://www.semanticscholar.org/"),
                                  GURL("https://www.sogou.com/"),
                                  GURL("https://www.startpage.com/"),
                                  GURL("https://www.webcrawler.com/"),
                                  GURL("https://www.wolframalpha.com/"),
                                  GURL("https://www.youtube.com/"),
                                  GURL("https://yandex.com/")};

  // Act

  // Assert
  for (const auto& url : urls) {
    EXPECT_TRUE(IsSearchEngine(url));
  }
}

TEST(HnsAdsSearchEngineUtilTest, IsNotSearchEngine) {
  // Arrange
  const GURL url = GURL("https://foobar.com/");

  // Act

  // Assert
  EXPECT_FALSE(IsSearchEngine(url));
}

TEST(HnsAdsSearchEngineUtilTest, IsNotSearchEngineWithInvalidUrl) {
  // Arrange
  const GURL url = GURL("INVALID");

  // Act

  // Assert
  EXPECT_FALSE(IsSearchEngine(url));
}

}  // namespace hns_ads
