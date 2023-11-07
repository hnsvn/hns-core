/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/targeting/geographical/subdivision/subdivision_targeting_util.h"

#include "testing/gtest/include/gtest/gtest.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads {

TEST(HnsAdsSubdivisionTargetingUtilTest, ShouldTargetSubdivisionCountryCode) {
  // Arrange

  // Act

  // Assert
  EXPECT_TRUE(ShouldTargetSubdivisionCountryCode(
      /*United States of America*/ "US"));
  EXPECT_TRUE(ShouldTargetSubdivisionCountryCode(/*Canada*/ "CA"));
}

TEST(HnsAdsSubdivisionTargetingUtilTest,
     ShouldNotTargetSubdivisionCountryCode) {
  // Arrange

  // Act

  // Assert
  EXPECT_FALSE(ShouldTargetSubdivisionCountryCode("XX"));
}

TEST(HnsAdsSubdivisionTargetingUtilTest, ShouldTargetSubdivision) {
  // Arrange

  // Act

  // Assert
  EXPECT_TRUE(ShouldTargetSubdivision(
      /*United States of America*/ "US", /*subdivision*/ "US-CA"));
}

TEST(HnsAdsSubdivisionTargetingUtilTest, ShouldNotTargetSubdivision) {
  // Arrange

  // Act

  // Assert
  EXPECT_FALSE(ShouldTargetSubdivision(
      /*United States of America*/ "US", /*subdivision*/ "US-XX"));
}

}  // namespace hns_ads
