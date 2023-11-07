/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/common/challenge_bypass_ristretto/verification_signature.h"

#include <sstream>

#include "hns/components/hns_ads/core/internal/common/challenge_bypass_ristretto/challenge_bypass_ristretto_unittest_constants.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_base.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads::cbr {

class HnsAdsVerificationSignatureTest : public UnitTestBase {};

TEST_F(HnsAdsVerificationSignatureTest, FailToInitialize) {
  // Arrange
  const VerificationSignature verification_signature;

  // Act

  // Assert
  EXPECT_FALSE(verification_signature.has_value());
}

TEST_F(HnsAdsVerificationSignatureTest, FailToInitializeWithEmptyBase64) {
  // Arrange
  const VerificationSignature verification_signature("");

  // Act

  // Assert
  EXPECT_FALSE(verification_signature.has_value());
}

TEST_F(HnsAdsVerificationSignatureTest, FailToInitializeWithInvalidBase64) {
  // Arrange
  const VerificationSignature verification_signature(kInvalidBase64);

  // Act

  // Assert
  EXPECT_FALSE(verification_signature.has_value());
}

TEST_F(HnsAdsVerificationSignatureTest, DecodeBase64) {
  // Arrange

  // Act
  const VerificationSignature verification_signature =
      VerificationSignature::DecodeBase64(kVerificationSignatureBase64);

  // Assert
  EXPECT_TRUE(verification_signature.has_value());
}

TEST_F(HnsAdsVerificationSignatureTest, FailToDecodeEmptyBase64) {
  // Arrange

  // Act
  const VerificationSignature verification_signature =
      VerificationSignature::DecodeBase64("");

  // Assert
  EXPECT_FALSE(verification_signature.has_value());
}

TEST_F(HnsAdsVerificationSignatureTest, FailToDecodeInvalidBase64) {
  // Arrange

  // Act
  const VerificationSignature verification_signature =
      VerificationSignature::DecodeBase64(kInvalidBase64);

  // Assert
  EXPECT_FALSE(verification_signature.has_value());
}

TEST_F(HnsAdsVerificationSignatureTest, EncodeBase64) {
  // Arrange
  const VerificationSignature verification_signature(
      kVerificationSignatureBase64);

  // Act

  // Assert
  EXPECT_EQ(kVerificationSignatureBase64,
            verification_signature.EncodeBase64());
}

TEST_F(HnsAdsVerificationSignatureTest, FailToEncodeBase64WhenUninitialized) {
  // Arrange
  const VerificationSignature verification_signature;

  // Act

  // Assert
  EXPECT_FALSE(verification_signature.EncodeBase64());
}

TEST_F(HnsAdsVerificationSignatureTest, IsEqual) {
  // Arrange
  const VerificationSignature verification_signature(
      kVerificationSignatureBase64);

  // Act

  // Assert
  EXPECT_EQ(verification_signature, verification_signature);
}

TEST_F(HnsAdsVerificationSignatureTest, IsEqualWhenUninitialized) {
  // Arrange
  const VerificationSignature verification_signature;

  // Act

  // Assert
  EXPECT_EQ(verification_signature, verification_signature);
}

TEST_F(HnsAdsVerificationSignatureTest, IsEmptyBase64Equal) {
  // Arrange
  const VerificationSignature verification_signature("");

  // Act

  // Assert
  EXPECT_EQ(verification_signature, verification_signature);
}

TEST_F(HnsAdsVerificationSignatureTest, IsInvalidBase64Equal) {
  // Arrange
  const VerificationSignature verification_signature(kInvalidBase64);

  // Act

  // Assert
  EXPECT_EQ(verification_signature, verification_signature);
}

TEST_F(HnsAdsVerificationSignatureTest, IsNotEqual) {
  // Arrange
  const VerificationSignature verification_signature(
      kVerificationSignatureBase64);

  // Act

  // Assert
  const VerificationSignature different_verification_signature(kInvalidBase64);
  EXPECT_NE(different_verification_signature, verification_signature);
}

TEST_F(HnsAdsVerificationSignatureTest, OutputStream) {
  // Arrange
  const VerificationSignature verification_signature(
      kVerificationSignatureBase64);

  // Act
  std::stringstream ss;
  ss << verification_signature;

  // Assert
  EXPECT_EQ(kVerificationSignatureBase64, ss.str());
}

TEST_F(HnsAdsVerificationSignatureTest, OutputStreamWhenUninitialized) {
  // Arrange
  const VerificationSignature verification_signature;

  // Act
  std::stringstream ss;
  ss << verification_signature;

  // Assert
  EXPECT_TRUE(ss.str().empty());
}

}  // namespace hns_ads::cbr
