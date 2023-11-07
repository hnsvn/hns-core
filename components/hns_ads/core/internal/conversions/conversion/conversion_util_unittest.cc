/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/conversions/conversion/conversion_util.h"

#include "hns/components/hns_ads/core/internal/conversions/conversion/conversion_info.h"
#include "hns/components/hns_ads/core/internal/conversions/types/verifiable_conversion/verifiable_conversion_unittest_constants.h"
#include "testing/gtest/include/gtest/gtest.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads {

TEST(HnsAdsConversionInfoUtilTest, ConversionTypeToString) {
  // Arrange
  const ConversionInfo conversion;

  // Act

  // Assert
  EXPECT_EQ("conversion", ConversionTypeToString(conversion));
}

TEST(HnsAdsConversionInfoUtilTest, VerifiableConversionTypeToString) {
  // Arrange
  ConversionInfo conversion;
  conversion.verifiable = {kVerifiableConversionId,
                           kVerifiableConversionAdvertiserPublicKey};

  // Act

  // Assert
  EXPECT_EQ("verifiable conversion", ConversionTypeToString(conversion));
}

}  // namespace hns_ads
