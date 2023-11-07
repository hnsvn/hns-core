/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/common/search_engine/search_engine_results_page_util.h"

#include "hns/components/hns_ads/core/internal/common/search_engine/search_engine_results_page_unittest_constants.h"
#include "testing/gtest/include/gtest/gtest.h"
#include "url/gurl.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads {

TEST(HnsAdsSearchEngineResultsPageUtilTest, IsSearchEngineResultsPage) {
  // Arrange

  // Act

  // Assert
  for (const auto& url : GetSearchEngineResultsPageUrls()) {
    EXPECT_TRUE(IsSearchEngineResultsPage(url));
  }
}

TEST(HnsAdsSearchEngineResultsPageUtilTest, IsNotSearchEngineResultsPage) {
  // Arrange

  // Act

  // Assert
  EXPECT_FALSE(IsSearchEngineResultsPage(GURL("https://hns.com/")));
}

TEST(HnsAdsSearchEngineResultsPageUtilTest,
     IsNotSearchEngineResultsPageWithInvalidUrl) {
  // Arrange

  // Act

  // Assert
  EXPECT_FALSE(IsSearchEngineResultsPage(GURL("INVALID")));
}

TEST(HnsAdsSearchEngineResultsPageUtilTest, ExtractSearchTermQueryValue) {
  // Arrange

  // Act

  // Assert
  for (const auto& url : GetSearchEngineResultsPageUrls()) {
    const absl::optional<std::string> search_term_query_value =
        ExtractSearchTermQueryValue(url);
    if (search_term_query_value) {
      EXPECT_EQ("foobar", search_term_query_value);
    }
  }
}

TEST(HnsAdsSearchEngineResultsPageUtilTest,
     FailToExtractSearchTermQueryValueFromUrlWithMissingQuery) {
  // Arrange

  // Act

  // Assert
  EXPECT_FALSE(ExtractSearchTermQueryValue(GURL("https://google.com/")));
}

TEST(HnsAdsSearchEngineResultsPageUtilTest,
     FailToExtractSearchTermQueryValueFromInvalidUrl) {
  // Arrange

  // Act

  // Assert
  EXPECT_FALSE(ExtractSearchTermQueryValue(GURL("INVALID")));
}

}  // namespace hns_ads
