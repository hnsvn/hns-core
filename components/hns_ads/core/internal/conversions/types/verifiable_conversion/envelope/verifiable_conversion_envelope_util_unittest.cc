/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/conversions/types/verifiable_conversion/envelope/verifiable_conversion_envelope_util.h"

#include "hns/components/hns_ads/core/internal/conversions/types/verifiable_conversion/envelope/verifiable_conversion_envelope_info.h"
#include "hns/components/hns_ads/core/internal/conversions/types/verifiable_conversion/envelope/verifiable_conversion_envelope_unittest_util.h"
#include "hns/components/hns_ads/core/internal/conversions/types/verifiable_conversion/envelope/verifiable_conversion_envelope_util_constants.h"
#include "hns/components/hns_ads/core/internal/conversions/types/verifiable_conversion/verifiable_conversion_info.h"
#include "hns/components/hns_ads/core/internal/conversions/types/verifiable_conversion/verifiable_conversion_unittest_constants.h"
#include "testing/gtest/include/gtest/gtest.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads {

TEST(HnsAdsVerifiableConversionEnvelopeUtilTest,
     SealVerifiableConversionEnvelope) {
  // Arrange
  const VerifiableConversionInfo verifiable_conversion{
      kVerifiableConversionId, kVerifiableConversionAdvertiserPublicKey};

  // Act
  const absl::optional<VerifiableConversionEnvelopeInfo>
      verifiable_conversion_envelope =
          SealVerifiableConversionEnvelope(verifiable_conversion);
  ASSERT_TRUE(verifiable_conversion_envelope);

  // Assert
  EXPECT_EQ(verifiable_conversion.id,
            OpenVerifiableConversionEnvelopeForTesting(
                *verifiable_conversion_envelope,
                kVerifiableConversionAdvertiserSecretKey));
}

TEST(HnsAdsVerifiableConversionEnvelopeUtilTest,
     DoNotSealEnvelopeWithShortMessage) {
  // Arrange
  VerifiableConversionInfo verifiable_conversion;
  verifiable_conversion.id =
      std::string(kMinVerifiableConversionEnvelopeMessageLength - 1, '-');
  verifiable_conversion.advertiser_public_key_base64 =
      kVerifiableConversionAdvertiserPublicKey;

  // Act

  // Assert
  EXPECT_FALSE(SealVerifiableConversionEnvelope(verifiable_conversion));
}

TEST(HnsAdsVerifiableConversionEnvelopeUtilTest,
     DoNotSealEnvelopeWithLongMessage) {
  // Arrange
  VerifiableConversionInfo verifiable_conversion;
  verifiable_conversion.id =
      std::string(kMaxVerifiableConversionEnvelopeMessageLength + 1, '-');
  verifiable_conversion.advertiser_public_key_base64 =
      kVerifiableConversionAdvertiserPublicKey;

  // Act

  // Assert
  EXPECT_FALSE(SealVerifiableConversionEnvelope(verifiable_conversion));
}

TEST(HnsAdsVerifiableConversionEnvelopeUtilTest,
     DoNotSealEnvelopeWithInvalidMessage) {
  // Arrange

  // Act

  // Assert
  EXPECT_FALSE(SealVerifiableConversionEnvelope(
      VerifiableConversionInfo{kInvalidVerifiableConversionId,
                               kVerifiableConversionAdvertiserPublicKey}));
}

TEST(HnsAdsVerifiableConversionEnvelopeUtilTest,
     DoNotSealEnvelopeWithInvalidPublicKey) {
  // Arrange

  // Act

  // Assert
  EXPECT_FALSE(SealVerifiableConversionEnvelope(VerifiableConversionInfo{
      kVerifiableConversionId,
      kInvalidVerifiableConversionAdvertiserPublicKey}));
}

}  // namespace hns_ads
