/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/common/challenge_bypass_ristretto/signing_key.h"

#include "hns/components/hns_ads/core/internal/common/challenge_bypass_ristretto/blinded_token.h"
#include "hns/components/hns_ads/core/internal/common/challenge_bypass_ristretto/blinded_token_unittest_util.h"
#include "hns/components/hns_ads/core/internal/common/challenge_bypass_ristretto/challenge_bypass_ristretto_unittest_constants.h"
#include "hns/components/hns_ads/core/internal/common/challenge_bypass_ristretto/public_key.h"
#include "hns/components/hns_ads/core/internal/common/challenge_bypass_ristretto/signed_token.h"
#include "hns/components/hns_ads/core/internal/common/challenge_bypass_ristretto/signed_token_unittest_util.h"
#include "hns/components/hns_ads/core/internal/common/challenge_bypass_ristretto/token_preimage.h"
#include "hns/components/hns_ads/core/internal/common/challenge_bypass_ristretto/token_preimage_unittest_util.h"
#include "hns/components/hns_ads/core/internal/common/challenge_bypass_ristretto/unblinded_token.h"
#include "hns/components/hns_ads/core/internal/common/challenge_bypass_ristretto/unblinded_token_unittest_util.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_base.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads::cbr {

class HnsAdsSigningKeyTest : public UnitTestBase {};

TEST_F(HnsAdsSigningKeyTest, FailToInitializeWithEmptyBase64) {
  // Arrange
  const SigningKey signing_key("");

  // Act

  // Assert
  EXPECT_FALSE(signing_key.has_value());
}

TEST_F(HnsAdsSigningKeyTest, FailToInitializeWithInvalidBase64) {
  // Arrange
  const SigningKey signing_key(kInvalidBase64);

  // Act

  // Assert
  EXPECT_FALSE(signing_key.has_value());
}

TEST_F(HnsAdsSigningKeyTest, DecodeBase64) {
  // Arrange

  // Act
  const SigningKey signing_key = SigningKey::DecodeBase64(kSigningKeyBase64);

  // Assert
  EXPECT_TRUE(signing_key.has_value());
}

TEST_F(HnsAdsSigningKeyTest, FailToDecodeEmptyBase64) {
  // Arrange

  // Act
  const SigningKey signing_key = SigningKey::DecodeBase64("");

  // Assert
  EXPECT_FALSE(signing_key.has_value());
}

TEST_F(HnsAdsSigningKeyTest, FailToDecodeInvalidBase64) {
  // Arrange

  // Act
  const SigningKey signing_key = SigningKey::DecodeBase64(kInvalidBase64);

  // Assert
  EXPECT_FALSE(signing_key.has_value());
}

TEST_F(HnsAdsSigningKeyTest, EncodeBase64) {
  // Arrange
  const SigningKey signing_key(kSigningKeyBase64);

  // Act

  // Assert
  EXPECT_EQ(kSigningKeyBase64, signing_key.EncodeBase64());
}

TEST_F(HnsAdsSigningKeyTest, Sign) {
  // Arrange
  const SigningKey signing_key(kSigningKeyBase64);

  // Act

  // Assert
  EXPECT_EQ(GetSignedTokenForTesting(),
            signing_key.Sign(GetBlindedTokenForTesting()));
}

TEST_F(HnsAdsSigningKeyTest, FailToSignWithInvalidBlindedToken) {
  // Arrange
  const SigningKey signing_key(kSigningKeyBase64);

  // Act

  // Assert
  EXPECT_FALSE(signing_key.Sign(GetInvalidBlindedTokenForTesting()));
}

TEST_F(HnsAdsSigningKeyTest, RederiveUnblindedToken) {
  // Arrange
  SigningKey signing_key(kSigningKeyBase64);

  // Act

  // Assert
  EXPECT_EQ(GetUnblindedTokenForTesting(),
            signing_key.RederiveUnblindedToken(GetTokenPreimageForTesting()));
}

TEST_F(HnsAdsSigningKeyTest,
       FailToRederiveUnblindedTokenWithInvalidTokenPreimage) {
  // Arrange
  SigningKey signing_key(kSigningKeyBase64);

  // Act

  // Assert
  EXPECT_FALSE(
      signing_key.RederiveUnblindedToken(GetInvalidTokenPreimageForTesting()));
}

TEST_F(HnsAdsSigningKeyTest, GetPublicKey) {
  // Arrange
  SigningKey signing_key(kSigningKeyBase64);

  // Act

  // Assert
  EXPECT_EQ(PublicKey(kPublicKeyBase64), signing_key.GetPublicKey());
}

TEST_F(HnsAdsSigningKeyTest, IsEqual) {
  // Arrange
  const SigningKey signing_key;

  // Act

  // Assert
  EXPECT_EQ(signing_key, signing_key);
}

TEST_F(HnsAdsSigningKeyTest, IsEmptyBase64Equal) {
  // Arrange
  const SigningKey signing_key("");

  // Act

  // Assert
  EXPECT_EQ(signing_key, signing_key);
}

TEST_F(HnsAdsSigningKeyTest, IsInvalidBase64Equal) {
  // Arrange
  const SigningKey signing_key(kInvalidBase64);

  // Act

  // Assert
  EXPECT_EQ(signing_key, signing_key);
}

TEST_F(HnsAdsSigningKeyTest, IsNotEqual) {
  // Arrange
  const SigningKey signing_key;

  // Act

  // Assert
  const SigningKey different_signing_key;
  EXPECT_NE(different_signing_key, signing_key);
}

TEST_F(HnsAdsSigningKeyTest, OutputStream) {
  // Arrange
  const SigningKey signing_key(kSigningKeyBase64);

  // Act
  std::stringstream ss;
  ss << signing_key;

  // Assert
  EXPECT_EQ(kSigningKeyBase64, ss.str());
}

}  // namespace hns_ads::cbr
