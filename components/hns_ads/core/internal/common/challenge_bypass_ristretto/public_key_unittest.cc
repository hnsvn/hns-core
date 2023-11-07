/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/common/challenge_bypass_ristretto/public_key.h"

#include <sstream>

#include "hns/components/hns_ads/core/internal/common/challenge_bypass_ristretto/challenge_bypass_ristretto_unittest_constants.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_base.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads::cbr {

class HnsAdsPublicKeyTest : public UnitTestBase {};

TEST_F(HnsAdsPublicKeyTest, FailToInitialize) {
  // Arrange
  const PublicKey public_key;

  // Act

  // Assert
  EXPECT_FALSE(public_key.has_value());
}

TEST_F(HnsAdsPublicKeyTest, FailToInitializeWithEmptyBase64) {
  // Arrange
  const PublicKey public_key("");

  // Act

  // Assert
  EXPECT_FALSE(public_key.has_value());
}

TEST_F(HnsAdsPublicKeyTest, FailToInitializeWithInvalidBase64) {
  // Arrange
  const PublicKey public_key(kInvalidBase64);

  // Act

  // Assert
  EXPECT_FALSE(public_key.has_value());
}

TEST_F(HnsAdsPublicKeyTest, DecodeBase64) {
  // Arrange

  // Act
  const PublicKey public_key = PublicKey::DecodeBase64(kPublicKeyBase64);

  // Assert
  EXPECT_TRUE(public_key.has_value());
}

TEST_F(HnsAdsPublicKeyTest, FailToDecodeEmptyBase64) {
  // Arrange

  // Act
  const PublicKey public_key = PublicKey::DecodeBase64("");

  // Assert
  EXPECT_FALSE(public_key.has_value());
}

TEST_F(HnsAdsPublicKeyTest, FailToDecodeInvalidBase64) {
  // Arrange

  // Act
  const PublicKey public_key = PublicKey::DecodeBase64(kInvalidBase64);

  // Assert
  EXPECT_FALSE(public_key.has_value());
}

TEST_F(HnsAdsPublicKeyTest, EncodeBase64) {
  // Arrange
  const PublicKey public_key(kPublicKeyBase64);

  // Act

  // Assert
  EXPECT_EQ(kPublicKeyBase64, public_key.EncodeBase64());
}

TEST_F(HnsAdsPublicKeyTest, FailToEncodeBase64WhenUninitialized) {
  // Arrange
  const PublicKey public_key;

  // Act

  // Assert
  EXPECT_FALSE(public_key.EncodeBase64());
}

TEST_F(HnsAdsPublicKeyTest, IsEqual) {
  // Arrange
  const PublicKey public_key(kPublicKeyBase64);

  // Act

  // Assert
  EXPECT_EQ(public_key, public_key);
}

TEST_F(HnsAdsPublicKeyTest, IsEqualWhenUninitialized) {
  // Arrange
  const PublicKey public_key;

  // Act

  // Assert
  EXPECT_EQ(public_key, public_key);
}

TEST_F(HnsAdsPublicKeyTest, IsEmptyBase64Equal) {
  // Arrange
  const PublicKey public_key("");

  // Act

  // Assert
  EXPECT_EQ(public_key, public_key);
}

TEST_F(HnsAdsPublicKeyTest, IsInvalidBase64Equal) {
  // Arrange
  const PublicKey public_key(kInvalidBase64);

  // Act

  // Assert
  EXPECT_EQ(public_key, public_key);
}

TEST_F(HnsAdsPublicKeyTest, IsNotEqual) {
  // Arrange
  const PublicKey public_key(kPublicKeyBase64);

  // Act

  // Assert
  const PublicKey different_public_key(kInvalidBase64);
  EXPECT_NE(different_public_key, public_key);
}

TEST_F(HnsAdsPublicKeyTest, OutputStream) {
  // Arrange
  const PublicKey public_key(kPublicKeyBase64);

  // Act
  std::stringstream ss;
  ss << public_key;

  // Assert
  EXPECT_EQ(kPublicKeyBase64, ss.str());
}

TEST_F(HnsAdsPublicKeyTest, OutputStreamWhenUninitialized) {
  // Arrange
  const PublicKey public_key;

  // Act
  std::stringstream ss;
  ss << public_key;

  // Assert
  EXPECT_TRUE(ss.str().empty());
}

}  // namespace hns_ads::cbr
