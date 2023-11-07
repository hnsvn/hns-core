/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/conversions/types/default_conversion/creative_set_conversion_url_pattern/creative_set_conversion_url_pattern_util.h"

#include "hns/components/hns_ads/core/internal/ads/ad_unittest_constants.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_base.h"
#include "hns/components/hns_ads/core/internal/creatives/conversions/creative_set_conversion_unittest_util.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads {

class HnsAdsCreativeConversionSetUrlPatternUtilTest : public UnitTestBase {};

TEST_F(HnsAdsCreativeConversionSetUrlPatternUtilTest,
       DoesCreativeSetConversionUrlPatternMatchRedirectChain) {
  // Arrange
  const CreativeSetConversionInfo creative_set_conversion =
      BuildCreativeSetConversionForTesting(
          kCreativeSetId,
          /*url_pattern*/ "https://foo.com/*",
          /*observation_window*/ base::Days(3));

  // Act

  // Assert
  EXPECT_TRUE(DoesCreativeSetConversionUrlPatternMatchRedirectChain(
      creative_set_conversion,
      /*redirect_chain*/ {GURL("https://foo.com/bar")}));
}

TEST_F(HnsAdsCreativeConversionSetUrlPatternUtilTest,
       DoesCreativeSetConversionUrlPatternNotMatchRedirectChain) {
  // Arrange
  const CreativeSetConversionInfo creative_set_conversion =
      BuildCreativeSetConversionForTesting(
          kCreativeSetId,
          /*url_pattern*/ "https://foo.com/*",
          /*observation_window*/ base::Days(3));

  // Act

  // Assert
  EXPECT_FALSE(DoesCreativeSetConversionUrlPatternMatchRedirectChain(
      creative_set_conversion,
      /*redirect_chain*/ {GURL("https://bar.com/foo")}));
}

}  // namespace hns_ads
