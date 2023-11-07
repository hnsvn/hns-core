/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/conversions/types/verifiable_conversion/verifiable_conversion_id_pattern/parsers/verifiable_conversion_id_html_parser_util.h"

#include "hns/components/hns_ads/core/internal/common/unittest/unittest_base.h"
#include "hns/components/hns_ads/core/internal/conversions/resource/conversion_resource_id_pattern_info.h"
#include "hns/components/hns_ads/core/internal/conversions/resource/conversion_resource_id_pattern_search_in_types.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads {

namespace {

constexpr char kMatchingHtml[] =
    R"(<html><div style="foo" id="xyzzy-id" class="bar">waldo</div></html>)";
constexpr char kMismatchingHtml[] =
    R"(<html><div style="foo" id="qux" class="bar">waldo</div></html>)";
constexpr char kEmptyHtml[] = "";

constexpr char kUrlPattern[] = "https://foo.com/bar";
constexpr ConversionResourceIdPatternSearchInType kSearchInType =
    ConversionResourceIdPatternSearchInType::kHtml;
constexpr char kIdPattern[] = R"(<div.*id="xyzzy-id".*>(.*)</div>)";

}  // namespace

class HnsAdsVerifiableConversionIdHtmlParserUtilTest : public UnitTestBase {};

TEST_F(HnsAdsVerifiableConversionIdHtmlParserUtilTest,
       ParseVerifableConversionIdFromHtml) {
  // Arrange

  // Act

  // Assert
  EXPECT_EQ("waldo",
            MaybeParseVerifableConversionIdFromHtml(
                kMatchingHtml, ConversionResourceIdPatternInfo{
                                   kUrlPattern, kSearchInType, kIdPattern}));
}

TEST_F(HnsAdsVerifiableConversionIdHtmlParserUtilTest,
       DoNotParseMismatchingVerifableConversionIdFromHtml) {
  // Arrange

  // Act

  // Assert
  EXPECT_FALSE(MaybeParseVerifableConversionIdFromHtml(
      kMismatchingHtml,
      ConversionResourceIdPatternInfo{kUrlPattern, kSearchInType, kIdPattern}));
}

TEST_F(HnsAdsVerifiableConversionIdHtmlParserUtilTest,
       DoNotParseVerifableConversionIdFromEmptyHtml) {
  // Arrange

  // Act

  // Assert
  EXPECT_FALSE(MaybeParseVerifableConversionIdFromHtml(
      kEmptyHtml,
      ConversionResourceIdPatternInfo{kUrlPattern, kSearchInType, kIdPattern}));
}

}  // namespace hns_ads
