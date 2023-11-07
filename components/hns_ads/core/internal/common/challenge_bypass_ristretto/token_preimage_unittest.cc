/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/common/challenge_bypass_ristretto/token_preimage.h"

#include <sstream>

#include "hns/components/hns_ads/core/internal/common/challenge_bypass_ristretto/challenge_bypass_ristretto_unittest_constants.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_base.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads::cbr {

class HnsAdsTokenPreimageTest : public UnitTestBase {};

TEST_F(HnsAdsTokenPreimageTest, FailToInitialize) {
  // Arrange
  const TokenPreimage token_preimage;

  // Act

  // Assert
  EXPECT_FALSE(token_preimage.has_value());
}

TEST_F(HnsAdsTokenPreimageTest, FailToInitializeWithEmptyBase64) {
  // Arrange
  const TokenPreimage token_preimage("");

  // Act

  // Assert
  EXPECT_FALSE(token_preimage.has_value());
}

TEST_F(HnsAdsTokenPreimageTest, FailToInitializeWithInvalidBase64) {
  // Arrange
  const TokenPreimage token_preimage(kInvalidBase64);

  // Act

  // Assert
  EXPECT_FALSE(token_preimage.has_value());
}

TEST_F(HnsAdsTokenPreimageTest, DecodeBase64) {
  // Arrange

  // Act
  const TokenPreimage token_preimage =
      TokenPreimage::DecodeBase64(kTokenPreimageBase64);

  // Assert
  EXPECT_TRUE(token_preimage.has_value());
}

TEST_F(HnsAdsTokenPreimageTest, FailToDecodeEmptyBase64) {
  // Arrange

  // Act
  const TokenPreimage token_preimage = TokenPreimage::DecodeBase64("");

  // Assert
  EXPECT_FALSE(token_preimage.has_value());
}

TEST_F(HnsAdsTokenPreimageTest, FailToDecodeInvalidBase64) {
  // Arrange

  // Act
  const TokenPreimage token_preimage =
      TokenPreimage::DecodeBase64(kInvalidBase64);

  // Assert
  EXPECT_FALSE(token_preimage.has_value());
}

TEST_F(HnsAdsTokenPreimageTest, EncodeBase64) {
  // Arrange
  const TokenPreimage token_preimage(kTokenPreimageBase64);

  // Act

  // Assert
  EXPECT_EQ(kTokenPreimageBase64, token_preimage.EncodeBase64());
}

TEST_F(HnsAdsTokenPreimageTest, FailToEncodeBase64WhenUninitialized) {
  // Arrange
  const TokenPreimage token_preimage;

  // Act

  // Assert
  EXPECT_FALSE(token_preimage.EncodeBase64());
}

TEST_F(HnsAdsTokenPreimageTest, IsEqual) {
  // Arrange
  const TokenPreimage token_preimage(kTokenPreimageBase64);

  // Act

  // Assert
  EXPECT_EQ(token_preimage, token_preimage);
}

TEST_F(HnsAdsTokenPreimageTest, IsEqualWhenUninitialized) {
  // Arrange
  const TokenPreimage token_preimage;

  // Act

  // Assert
  EXPECT_EQ(token_preimage, token_preimage);
}

TEST_F(HnsAdsTokenPreimageTest, IsEmptyBase64Equal) {
  // Arrange
  const TokenPreimage token_preimage("");

  // Act

  // Assert
  EXPECT_EQ(token_preimage, token_preimage);
}

TEST_F(HnsAdsTokenPreimageTest, IsInvalidBase64Equal) {
  // Arrange
  const TokenPreimage token_preimage(kInvalidBase64);

  // Act

  // Assert
  EXPECT_EQ(token_preimage, token_preimage);
}

TEST_F(HnsAdsTokenPreimageTest, IsNotEqual) {
  // Arrange
  const TokenPreimage token_preimage(kTokenPreimageBase64);

  // Act

  // Assert
  const TokenPreimage different_token_preimage(kInvalidBase64);
  EXPECT_NE(different_token_preimage, token_preimage);
}

TEST_F(HnsAdsTokenPreimageTest, OutputStream) {
  // Arrange
  const TokenPreimage token_preimage(kTokenPreimageBase64);

  // Act
  std::stringstream ss;
  ss << token_preimage;

  // Assert
  EXPECT_EQ(kTokenPreimageBase64, ss.str());
}

TEST_F(HnsAdsTokenPreimageTest, OutputStreamWhenUninitialized) {
  // Arrange
  const TokenPreimage token_preimage;

  // Act
  std::stringstream ss;
  ss << token_preimage;

  // Assert
  EXPECT_TRUE(ss.str().empty());
}

}  // namespace hns_ads::cbr
