/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/conversions/actions/conversion_action_types_util.h"

#include "hns/components/hns_ads/core/public/confirmation_type.h"
#include "testing/gtest/include/gtest/gtest.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads {

TEST(HnsAdsConversionActionTypesUtilTest, ToViewThroughConversionActionType) {
  // Arrange

  // Act

  // Assert
  EXPECT_EQ(ConversionActionType::kViewThrough,
            ToConversionActionType(ConfirmationType::kViewed));
}

TEST(HnsAdsConversionActionTypesUtilTest,
     ToClickThroughConversionActionType) {
  // Arrange

  // Act

  // Assert
  EXPECT_EQ(ConversionActionType::kClickThrough,
            ToConversionActionType(ConfirmationType::kClicked));
}

TEST(HnsAdsConversionActionTypesUtilTest,
     StringToViewThroughConversionActionType) {
  // Arrange

  // Act

  // Assert
  EXPECT_EQ(ConversionActionType::kViewThrough,
            StringToConversionActionType("view"));
}

TEST(HnsAdsConversionActionTypesUtilTest,
     StringToClickThroughConversionActionType) {
  // Arrange

  // Act

  // Assert
  EXPECT_EQ(ConversionActionType::kClickThrough,
            StringToConversionActionType("click"));
}

TEST(HnsAdsConversionActionTypesUtilTest,
     ViewThroughConversionActionTypeToString) {
  // Arrange

  // Act

  // Assert
  EXPECT_EQ("view",
            ConversionActionTypeToString(ConversionActionType::kViewThrough));
}

TEST(HnsAdsConversionActionTypesUtilTest,
     ClickThroughConversionActionTypeToString) {
  // Arrange

  // Act

  // Assert
  EXPECT_EQ("click",
            ConversionActionTypeToString(ConversionActionType::kClickThrough));
}

}  // namespace hns_ads
