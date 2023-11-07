/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/public/ad_info.h"

#include "hns/components/hns_ads/core/internal/ads/ad_unittest_util.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_base.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads {

class HnsAdsAdInfoTest : public UnitTestBase {};

TEST_F(HnsAdsAdInfoTest, IsValid) {
  // Arrange
  const AdInfo ad = BuildAdForTesting(AdType::kNotificationAd,
                                      /*should_use_random_uuids*/ true);

  // Act

  // Assert
  EXPECT_TRUE(ad.IsValid());
}

TEST_F(HnsAdsAdInfoTest, IsInvalid) {
  // Arrange
  const AdInfo ad;

  // Act

  // Assert
  EXPECT_FALSE(ad.IsValid());
}

}  // namespace hns_ads
