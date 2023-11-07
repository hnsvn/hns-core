/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/common/locale/country_code_anonymity_util.h"

#include "testing/gtest/include/gtest/gtest.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads::locale {

TEST(HnsAdsCountryCodeUtilTest, IsCountryCodeMemberOfAnonymitySet) {
  // Arrange

  // Act

  // Assert
  EXPECT_TRUE(IsCountryCodeMemberOfAnonymitySet("US"));
}

TEST(HnsAdsCountryCodeUtilTest, IsCountryCodeNotMemberOfAnonymitySet) {
  // Arrange

  // Act

  // Assert
  EXPECT_FALSE(IsCountryCodeMemberOfAnonymitySet("XX"));
}

TEST(HnsAdsCountryCodeUtilTest, ShouldClassifyCountryCodeAsOther) {
  // Arrange

  // Act

  // Assert
  EXPECT_TRUE(ShouldClassifyCountryCodeAsOther("CX"));
}

TEST(HnsAdsCountryCodeUtilTest, ShouldNotClassifyCountryCodeAsOther) {
  // Arrange

  // Act

  // Assert
  EXPECT_FALSE(ShouldClassifyCountryCodeAsOther("XX"));
}

}  // namespace hns_ads::locale
