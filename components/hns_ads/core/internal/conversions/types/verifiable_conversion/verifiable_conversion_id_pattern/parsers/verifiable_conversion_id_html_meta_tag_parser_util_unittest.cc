/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/conversions/types/verifiable_conversion/verifiable_conversion_id_pattern/parsers/verifiable_conversion_id_html_meta_tag_parser_util.h"

#include "hns/components/hns_ads/core/internal/common/unittest/unittest_base.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads {

class HnsAdsVerifiableConversionIdHtmlMetaTagParserUtilTest
    : public UnitTestBase {};

TEST_F(HnsAdsVerifiableConversionIdHtmlMetaTagParserUtilTest,
       ParseVerifableConversionIdFromHtmlMetaTag) {
  // Arrange

  // Act

  // Assert
  EXPECT_EQ("fred",
            MaybeParseVerifableConversionIdFromHtmlMetaTag(
                /*html*/ R"(<meta name="ad-conversion-id" content="fred">)"));
}

TEST_F(HnsAdsVerifiableConversionIdHtmlMetaTagParserUtilTest,
       DoNotParseMismatchingVerifableConversionIdFromEmptyHtmlMetaTag) {
  // Arrange

  // Act

  // Assert
  EXPECT_FALSE(MaybeParseVerifableConversionIdFromHtmlMetaTag(
      /*html*/ R"(<meta name="foo" content="bar">)"));
}

TEST_F(HnsAdsVerifiableConversionIdHtmlMetaTagParserUtilTest,
       DoNotParseVerifableConversionIdFromEmptyHtmlMetaTag) {
  // Arrange

  // Act

  // Assert
  EXPECT_FALSE(MaybeParseVerifableConversionIdFromHtmlMetaTag(/*html*/ {}));
}

}  // namespace hns_ads
