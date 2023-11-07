/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/common/subdivision/subdivision_util.h"

#include "testing/gtest/include/gtest/gtest.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads {

TEST(HnsAdsSubdivisionUtilTest, GetSubdivisionCountryCode) {
  // Arrange

  // Act

  // Assert
  EXPECT_EQ("US", GetSubdivisionCountryCode(/*subdivision*/ "US-CA"));
}

TEST(HnsAdsSubdivisionUtilTest, DoNotGetSubdivisionCountryCode) {
  // Arrange

  // Act

  // Assert
  EXPECT_FALSE(GetSubdivisionCountryCode({}));
}

TEST(HnsAdsSubdivisionUtilTest, GetSubdivisionCode) {
  // Arrange

  // Act

  // Assert
  EXPECT_EQ("CA", GetSubdivisionCode(/*subdivision*/ "US-CA"));
}

TEST(HnsAdsSubdivisionUtilTest, DoNotGetSubdivisionCode) {
  // Arrange

  // Act

  // Assert
  EXPECT_FALSE(GetSubdivisionCode({}));
}

}  // namespace hns_ads
