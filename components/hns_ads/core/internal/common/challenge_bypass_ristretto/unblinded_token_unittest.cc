/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/common/challenge_bypass_ristretto/unblinded_token.h"

#include "hns/components/hns_ads/core/internal/common/challenge_bypass_ristretto/challenge_bypass_ristretto_unittest_constants.h"
#include "hns/components/hns_ads/core/internal/common/challenge_bypass_ristretto/token_preimage.h"
#include "hns/components/hns_ads/core/internal/common/challenge_bypass_ristretto/token_preimage_unittest_util.h"
#include "hns/components/hns_ads/core/internal/common/challenge_bypass_ristretto/verification_key.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_base.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads::cbr {

class HnsAdsUnblindedTokenTest : public UnitTestBase {};

TEST_F(HnsAdsUnblindedTokenTest, FailToInitialize) {
  // Arrange
  const UnblindedToken unblinded_token;

  // Act

  // Assert
  EXPECT_FALSE(unblinded_token.has_value());
}

TEST_F(HnsAdsUnblindedTokenTest, FailToInitializeWithEmptyBase64) {
  // Arrange
  const UnblindedToken unblinded_token("");

  // Act

  // Assert
  EXPECT_FALSE(unblinded_token.has_value());
}

TEST_F(HnsAdsUnblindedTokenTest, FailToInitializeWithInvalidBase64) {
  // Arrange
  const UnblindedToken unblinded_token(kInvalidBase64);

  // Act

  // Assert
  EXPECT_FALSE(unblinded_token.has_value());
}

TEST_F(HnsAdsUnblindedTokenTest, DecodeBase64) {
  // Arrange

  // Act
  const UnblindedToken unblinded_token =
      UnblindedToken::DecodeBase64(kUnblindedTokenBase64);

  // Assert
  EXPECT_TRUE(unblinded_token.has_value());
}

TEST_F(HnsAdsUnblindedTokenTest, FailToDecodeEmptyBase64) {
  // Arrange

  // Act
  const UnblindedToken unblinded_token = UnblindedToken::DecodeBase64("");

  // Assert
  EXPECT_FALSE(unblinded_token.has_value());
}

TEST_F(HnsAdsUnblindedTokenTest, FailToDecodeInvalidBase64) {
  // Arrange

  // Act
  const UnblindedToken unblinded_token =
      UnblindedToken::DecodeBase64(kInvalidBase64);

  // Assert
  EXPECT_FALSE(unblinded_token.has_value());
}

TEST_F(HnsAdsUnblindedTokenTest, EncodeBase64) {
  // Arrange
  const UnblindedToken unblinded_token(kUnblindedTokenBase64);

  // Act

  // Assert
  EXPECT_EQ(kUnblindedTokenBase64, unblinded_token.EncodeBase64());
}

TEST_F(HnsAdsUnblindedTokenTest, FailToEncodeBase64WhenUninitialized) {
  // Arrange
  const UnblindedToken unblinded_token;

  // Act

  // Assert
  EXPECT_FALSE(unblinded_token.EncodeBase64());
}

TEST_F(HnsAdsUnblindedTokenTest, IsEqual) {
  // Arrange
  const UnblindedToken unblinded_token(kUnblindedTokenBase64);

  // Act

  // Assert
  EXPECT_EQ(unblinded_token, unblinded_token);
}

TEST_F(HnsAdsUnblindedTokenTest, IsEqualWhenUninitialized) {
  // Arrange
  const UnblindedToken unblinded_token;

  // Act

  // Assert
  EXPECT_EQ(unblinded_token, unblinded_token);
}

TEST_F(HnsAdsUnblindedTokenTest, IsEmptyBase64Equal) {
  // Arrange
  const UnblindedToken unblinded_token("");

  // Act

  // Assert
  EXPECT_EQ(unblinded_token, unblinded_token);
}

TEST_F(HnsAdsUnblindedTokenTest, IsInvalidBase64Equal) {
  // Arrange
  const UnblindedToken unblinded_token(kInvalidBase64);

  // Act

  // Assert
  EXPECT_EQ(unblinded_token, unblinded_token);
}

TEST_F(HnsAdsUnblindedTokenTest, IsNotEqual) {
  // Arrange
  const UnblindedToken unblinded_token(kUnblindedTokenBase64);

  // Act

  // Assert
  const UnblindedToken different_blinded_token(kInvalidBase64);
  EXPECT_NE(different_blinded_token, unblinded_token);
}

TEST_F(HnsAdsUnblindedTokenTest, OutputStream) {
  // Arrange
  const UnblindedToken unblinded_token(kUnblindedTokenBase64);

  // Act
  std::stringstream ss;
  ss << unblinded_token;

  // Assert
  EXPECT_EQ(kUnblindedTokenBase64, ss.str());
}

TEST_F(HnsAdsUnblindedTokenTest, OutputStreamWhenUninitialized) {
  // Arrange
  const UnblindedToken unblinded_token;

  // Act
  std::stringstream ss;
  ss << unblinded_token;

  // Assert
  EXPECT_TRUE(ss.str().empty());
}

TEST_F(HnsAdsUnblindedTokenTest, DeriveVerificationKey) {
  // Arrange
  const UnblindedToken unblinded_token(kUnblindedTokenBase64);

  // Act

  // Assert
  EXPECT_TRUE(unblinded_token.DeriveVerificationKey());
}

TEST_F(HnsAdsUnblindedTokenTest,
       FailToDeriveVerificationKeyWhenUninitialized) {
  // Arrange
  const UnblindedToken unblinded_token;

  // Act

  // Assert
  EXPECT_FALSE(unblinded_token.DeriveVerificationKey());
}

TEST_F(HnsAdsUnblindedTokenTest,
       FailToDeriveVerificationKeyWithInvalidUnblindedToken) {
  // Arrange
  const UnblindedToken unblinded_token(kInvalidBase64);

  // Act

  // Assert
  EXPECT_FALSE(unblinded_token.DeriveVerificationKey());
}

TEST_F(HnsAdsUnblindedTokenTest, GetTokenPreimage) {
  // Arrange
  const UnblindedToken unblinded_token(kUnblindedTokenBase64);

  // Act

  // Assert
  EXPECT_EQ(GetTokenPreimageForTesting(), unblinded_token.GetTokenPreimage());
}

TEST_F(HnsAdsUnblindedTokenTest, FailToGetTokenPreimageWhenUninitialized) {
  // Arrange
  const UnblindedToken unblinded_token;

  // Act

  // Assert
  EXPECT_FALSE(unblinded_token.GetTokenPreimage());
}

TEST_F(HnsAdsUnblindedTokenTest,
       FailToGetTokenPreimageWithInvalidUnblindedToken) {
  // Arrange
  const UnblindedToken unblinded_token(kInvalidBase64);

  // Act

  // Assert
  EXPECT_FALSE(unblinded_token.GetTokenPreimage());
}

}  // namespace hns_ads::cbr
