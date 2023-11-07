/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/common/challenge_bypass_ristretto/blinded_token.h"

#include <sstream>

#include "hns/components/hns_ads/core/internal/common/challenge_bypass_ristretto/challenge_bypass_ristretto_unittest_constants.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_base.h"
#include "testing/gtest/include/gtest/gtest.h"
#include "third_party/abseil-cpp/absl/types/optional.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads::cbr {

class HnsAdsBlindedTokenTest : public UnitTestBase {};

TEST_F(HnsAdsBlindedTokenTest, FailToInitialize) {
  // Arrange
  const BlindedToken blinded_token;

  // Act

  // Assert
  EXPECT_FALSE(blinded_token.has_value());
}

TEST_F(HnsAdsBlindedTokenTest, FailToInitializeWithEmptyBase64) {
  // Arrange
  const BlindedToken blinded_token("");

  // Act

  // Assert
  EXPECT_FALSE(blinded_token.has_value());
}

TEST_F(HnsAdsBlindedTokenTest, FailToInitializeWithInvalidBase64) {
  // Arrange
  const BlindedToken blinded_token(kInvalidBase64);

  // Act

  // Assert
  EXPECT_FALSE(blinded_token.has_value());
}

TEST_F(HnsAdsBlindedTokenTest, DecodeBase64) {
  // Arrange

  // Act
  const BlindedToken blinded_token =
      BlindedToken::DecodeBase64(kBlindedTokenBase64);

  // Assert
  EXPECT_TRUE(blinded_token.has_value());
}

TEST_F(HnsAdsBlindedTokenTest, FailToDecodeEmptyBase64) {
  // Arrange

  // Act
  const BlindedToken blinded_token = BlindedToken::DecodeBase64("");

  // Assert
  EXPECT_FALSE(blinded_token.has_value());
}

TEST_F(HnsAdsBlindedTokenTest, FailToDecodeInvalidBase64) {
  // Arrange

  // Act
  const BlindedToken blinded_token = BlindedToken::DecodeBase64(kInvalidBase64);

  // Assert
  EXPECT_FALSE(blinded_token.has_value());
}

TEST_F(HnsAdsBlindedTokenTest, EncodeBase64) {
  // Arrange
  const BlindedToken blinded_token(kBlindedTokenBase64);

  // Act

  // Assert
  EXPECT_EQ(kBlindedTokenBase64, blinded_token.EncodeBase64());
}

TEST_F(HnsAdsBlindedTokenTest, FailToEncodeBase64WhenUninitialized) {
  // Arrange
  const BlindedToken blinded_token;

  // Act

  // Assert
  EXPECT_FALSE(blinded_token.EncodeBase64());
}

TEST_F(HnsAdsBlindedTokenTest, IsEqual) {
  // Arrange
  const BlindedToken blinded_token(kBlindedTokenBase64);

  // Act

  // Assert
  EXPECT_EQ(blinded_token, blinded_token);
}

TEST_F(HnsAdsBlindedTokenTest, IsEqualWhenUninitialized) {
  // Arrange
  const BlindedToken blinded_token;

  // Act

  // Assert
  EXPECT_EQ(blinded_token, blinded_token);
}

TEST_F(HnsAdsBlindedTokenTest, IsEmptyBase64Equal) {
  // Arrange
  const BlindedToken blinded_token("");

  // Act

  // Assert
  EXPECT_EQ(blinded_token, blinded_token);
}

TEST_F(HnsAdsBlindedTokenTest, IsInvalidBase64Equal) {
  // Arrange
  const BlindedToken blinded_token(kInvalidBase64);

  // Act

  // Assert
  EXPECT_EQ(blinded_token, blinded_token);
}

TEST_F(HnsAdsBlindedTokenTest, IsNotEqual) {
  // Arrange

  // Act
  const BlindedToken blinded_token(kBlindedTokenBase64);
  const BlindedToken different_blinded_token(kInvalidBase64);

  // Assert
  EXPECT_NE(different_blinded_token, blinded_token);
}

TEST_F(HnsAdsBlindedTokenTest, OutputStream) {
  // Arrange
  const BlindedToken blinded_token(kBlindedTokenBase64);

  // Act
  std::stringstream ss;
  ss << blinded_token;

  // Assert
  EXPECT_EQ(kBlindedTokenBase64, ss.str());
}

TEST_F(HnsAdsBlindedTokenTest, OutputStreamWhenUninitialized) {
  // Arrange
  const BlindedToken blinded_token;

  // Act
  std::stringstream ss;
  ss << blinded_token;

  // Assert
  EXPECT_TRUE(ss.str().empty());
}

}  // namespace hns_ads::cbr
