/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/targeting/geographical/subdivision/subdivision_url_request_json_reader_util.h"

#include "testing/gtest/include/gtest/gtest.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads::json::reader {

TEST(HnsAdsSubdivisionUrlRequestJsonReaderUtilTest, ParseValidJson) {
  // Arrange

  // Act

  // Assert
  EXPECT_EQ("US-CA", ParseSubdivision(R"({"country":"US","region":"CA"})"));
}

TEST(HnsAdsSubdivisionUrlRequestJsonReaderUtilTest, DoNotParseInvalidJson) {
  // Arrange

  // Act

  // Assert
  EXPECT_FALSE(ParseSubdivision("{INVALID}"));
}

TEST(HnsAdsSubdivisionUrlRequestJsonReaderUtilTest,
     DoNotParseIfMissingCountry) {
  // Arrange

  // Act

  // Assert
  EXPECT_FALSE(ParseSubdivision(R"({"region":"CA"})"));
}

TEST(HnsAdsSubdivisionUrlRequestJsonReaderUtilTest,
     DoNotParseifEmptyCountry) {
  // Arrange

  // Act

  // Assert
  EXPECT_FALSE(ParseSubdivision(R"({"country":"","region":"CA"})"));
}

TEST(HnsAdsSubdivisionUrlRequestJsonReaderUtilTest,
     DoNotParseIfMissingRegion) {
  // Arrange

  // Act

  // Assert
  EXPECT_FALSE(ParseSubdivision(R"({"country":"US"})"));
}

TEST(HnsAdsSubdivisionUrlRequestJsonReaderUtilTest, DoNotParseIfEmptyRegion) {
  // Arrange

  // Act

  // Assert
  EXPECT_FALSE(ParseSubdivision(R"({"country":"US","region":""})"));
}

}  // namespace hns_ads::json::reader
