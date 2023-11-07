/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/common/challenge_bypass_ristretto/signed_token.h"

#include <sstream>

#include "hns/components/hns_ads/core/internal/common/challenge_bypass_ristretto/challenge_bypass_ristretto_unittest_constants.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_base.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads::cbr {

class HnsAdsSignedTokenTest : public UnitTestBase {};

TEST_F(HnsAdsSignedTokenTest, FailToInitialize) {
  // Arrange
  const SignedToken signed_token;

  // Act

  // Assert
  EXPECT_FALSE(signed_token.has_value());
}

TEST_F(HnsAdsSignedTokenTest, FailToInitializeWithEmptyBase64) {
  // Arrange
  const SignedToken signed_token("");

  // Act

  // Assert
  EXPECT_FALSE(signed_token.has_value());
}

TEST_F(HnsAdsSignedTokenTest, FailToInitializeWithInvalidBase64) {
  // Arrange
  const SignedToken signed_token(kInvalidBase64);

  // Act

  // Assert
  EXPECT_FALSE(signed_token.has_value());
}

TEST_F(HnsAdsSignedTokenTest, DecodeBase64) {
  // Arrange

  // Act
  const SignedToken signed_token =
      SignedToken::DecodeBase64(kSignedTokenBase64);

  // Assert
  EXPECT_TRUE(signed_token.has_value());
}

TEST_F(HnsAdsSignedTokenTest, FailToDecodeEmptyBase64) {
  // Arrange

  // Act
  const SignedToken signed_token = SignedToken::DecodeBase64("");

  // Assert
  EXPECT_FALSE(signed_token.has_value());
}

TEST_F(HnsAdsSignedTokenTest, FailToDecodeInvalidBase64) {
  // Arrange

  // Act
  const SignedToken signed_token = SignedToken::DecodeBase64(kInvalidBase64);

  // Assert
  EXPECT_FALSE(signed_token.has_value());
}

TEST_F(HnsAdsSignedTokenTest, EncodeBase64) {
  // Arrange
  const SignedToken signed_token(kSignedTokenBase64);

  // Act

  // Assert
  EXPECT_EQ(kSignedTokenBase64, signed_token.EncodeBase64());
}

TEST_F(HnsAdsSignedTokenTest, FailToEncodeBase64WhenUninitialized) {
  // Arrange
  const SignedToken signed_token;

  // Act

  // Assert
  EXPECT_FALSE(signed_token.EncodeBase64());
}

TEST_F(HnsAdsSignedTokenTest, IsEqual) {
  // Arrange
  const SignedToken signed_token(kSignedTokenBase64);

  // Act

  // Assert
  EXPECT_EQ(signed_token, signed_token);
}

TEST_F(HnsAdsSignedTokenTest, IsEqualWhenUninitialized) {
  // Arrange
  const SignedToken signed_token;

  // Act

  // Assert
  EXPECT_EQ(signed_token, signed_token);
}

TEST_F(HnsAdsSignedTokenTest, IsEmptyBase64Equal) {
  // Arrange
  const SignedToken signed_token("");

  // Act

  // Assert
  EXPECT_EQ(signed_token, signed_token);
}

TEST_F(HnsAdsSignedTokenTest, IsInvalidBase64Equal) {
  // Arrange
  const SignedToken signed_token(kInvalidBase64);

  // Act

  // Assert
  EXPECT_EQ(signed_token, signed_token);
}

TEST_F(HnsAdsSignedTokenTest, IsNotEqual) {
  // Arrange
  const SignedToken signed_token(kSignedTokenBase64);

  // Act

  // Assert
  const SignedToken different_signed_token(kInvalidBase64);
  EXPECT_NE(different_signed_token, signed_token);
}

TEST_F(HnsAdsSignedTokenTest, OutputStream) {
  // Arrange
  const SignedToken signed_token(kSignedTokenBase64);

  // Act
  std::stringstream ss;
  ss << signed_token;

  // Assert
  EXPECT_EQ(kSignedTokenBase64, ss.str());
}

TEST_F(HnsAdsSignedTokenTest, OutputStreamWhenUninitialized) {
  // Arrange
  const SignedToken signed_token;

  // Act
  std::stringstream ss;
  ss << signed_token;

  // Assert
  EXPECT_TRUE(ss.str().empty());
}

}  // namespace hns_ads::cbr
