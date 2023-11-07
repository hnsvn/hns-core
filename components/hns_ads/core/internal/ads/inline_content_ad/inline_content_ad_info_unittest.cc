/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/public/ads/inline_content_ad_info.h"

#include "hns/components/hns_ads/core/internal/common/unittest/unittest_base.h"
#include "hns/components/hns_ads/core/internal/creatives/inline_content_ads/creative_inline_content_ad_info.h"
#include "hns/components/hns_ads/core/internal/creatives/inline_content_ads/creative_inline_content_ad_unittest_util.h"
#include "hns/components/hns_ads/core/internal/creatives/inline_content_ads/inline_content_ad_builder.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads {

class HnsAdsInlineContentAdInfoTest : public UnitTestBase {};

TEST_F(HnsAdsInlineContentAdInfoTest, IsValid) {
  // Arrange
  const CreativeInlineContentAdInfo creative_ad =
      BuildCreativeInlineContentAdForTesting(/*should_use_random_uuids*/ true);
  const InlineContentAdInfo ad = BuildInlineContentAd(creative_ad);

  // Act

  // Assert
  EXPECT_TRUE(ad.IsValid());
}

TEST_F(HnsAdsInlineContentAdInfoTest, IsInvalid) {
  // Arrange
  const InlineContentAdInfo ad;

  // Act

  // Assert
  EXPECT_FALSE(ad.IsValid());
}

}  // namespace hns_ads
