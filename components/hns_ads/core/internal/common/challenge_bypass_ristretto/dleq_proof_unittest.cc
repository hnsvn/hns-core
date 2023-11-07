/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/common/challenge_bypass_ristretto/dleq_proof.h"

#include <sstream>

#include "hns/components/hns_ads/core/internal/common/challenge_bypass_ristretto/blinded_token.h"
#include "hns/components/hns_ads/core/internal/common/challenge_bypass_ristretto/blinded_token_unittest_util.h"
#include "hns/components/hns_ads/core/internal/common/challenge_bypass_ristretto/challenge_bypass_ristretto_unittest_constants.h"
#include "hns/components/hns_ads/core/internal/common/challenge_bypass_ristretto/public_key.h"
#include "hns/components/hns_ads/core/internal/common/challenge_bypass_ristretto/public_key_unittest_util.h"
#include "hns/components/hns_ads/core/internal/common/challenge_bypass_ristretto/signed_token.h"
#include "hns/components/hns_ads/core/internal/common/challenge_bypass_ristretto/signed_token_unittest_util.h"
#include "hns/components/hns_ads/core/internal/common/challenge_bypass_ristretto/signing_key.h"
#include "hns/components/hns_ads/core/internal/common/challenge_bypass_ristretto/signing_key_unittest_util.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_base.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads::cbr {

class HnsAdsDLEQProofTest : public UnitTestBase {};

TEST_F(HnsAdsDLEQProofTest, FailToInitialize) {
  // Arrange
  const DLEQProof dleq_proof;

  // Act

  // Assert
  EXPECT_FALSE(dleq_proof.has_value());
}

TEST_F(HnsAdsDLEQProofTest, FailToInitializeWithEmptyBase64) {
  // Arrange
  const DLEQProof dleq_proof("");

  // Act

  // Assert
  EXPECT_FALSE(dleq_proof.has_value());
}

TEST_F(HnsAdsDLEQProofTest, FailToInitializeWithInvalidBase64) {
  // Arrange
  const DLEQProof dleq_proof(kInvalidBase64);

  // Act

  // Assert
  EXPECT_FALSE(dleq_proof.has_value());
}

TEST_F(HnsAdsDLEQProofTest, FailToInitializeWithInvalidBlindedToken) {
  // Arrange
  const DLEQProof dleq_proof(GetInvalidBlindedTokenForTesting(),
                             GetSignedTokenForTesting(),
                             GetSigningKeyForTesting());

  // Act

  // Assert
  EXPECT_FALSE(dleq_proof.has_value());
}

TEST_F(HnsAdsDLEQProofTest, FailToInitializeWithInvalidSignedToken) {
  // Arrange
  const DLEQProof dleq_proof(GetBlindedTokenForTesting(),
                             GetInvalidSignedTokenForTesting(),
                             GetSigningKeyForTesting());

  // Act

  // Assert
  EXPECT_FALSE(dleq_proof.has_value());
}

TEST_F(HnsAdsDLEQProofTest, FailToInitializeWithInvalidSigningKey) {
  // Arrange
  const DLEQProof dleq_proof(GetBlindedTokenForTesting(),
                             GetSignedTokenForTesting(),
                             GetInvalidSigningKeyForTesting());

  // Act

  // Assert
  EXPECT_FALSE(dleq_proof.has_value());
}

TEST_F(HnsAdsDLEQProofTest, DecodeBase64) {
  // Arrange

  // Act
  const DLEQProof dleq_proof = DLEQProof::DecodeBase64(kDLEQProofBase64);

  // Assert
  EXPECT_TRUE(dleq_proof.has_value());
}

TEST_F(HnsAdsDLEQProofTest, FailToDecodeEmptyBase64) {
  // Arrange

  // Act
  const DLEQProof dleq_proof = DLEQProof::DecodeBase64("");

  // Assert
  EXPECT_FALSE(dleq_proof.has_value());
}

TEST_F(HnsAdsDLEQProofTest, FailToDecodeInvalidBase64) {
  // Arrange

  // Act
  const DLEQProof dleq_proof = DLEQProof::DecodeBase64(kInvalidBase64);

  // Assert
  EXPECT_FALSE(dleq_proof.has_value());
}

TEST_F(HnsAdsDLEQProofTest, EncodeBase64) {
  // Arrange
  const DLEQProof dleq_proof(kDLEQProofBase64);

  // Act

  // Assert
  EXPECT_EQ(kDLEQProofBase64, dleq_proof.EncodeBase64());
}

TEST_F(HnsAdsDLEQProofTest, FailToEncodeBase64WhenUninitialized) {
  // Arrange
  const DLEQProof dleq_proof;

  // Act

  // Assert
  EXPECT_FALSE(dleq_proof.EncodeBase64());
}

TEST_F(HnsAdsDLEQProofTest, Verify) {
  // Arrange
  DLEQProof dleq_proof(kDLEQProofBase64);

  // Act

  // Assert
  EXPECT_TRUE(dleq_proof.Verify(GetBlindedTokenForTesting(),
                                GetSignedTokenForTesting(),
                                GetPublicKeyForTesting()));
}

TEST_F(HnsAdsDLEQProofTest, FailToVerifyWhenUninitialized) {
  // Arrange
  DLEQProof dleq_proof;

  // Act

  // Assert
  EXPECT_FALSE(dleq_proof.Verify(GetBlindedTokenForTesting(),
                                 GetSignedTokenForTesting(),
                                 GetPublicKeyForTesting()));
}

TEST_F(HnsAdsDLEQProofTest, FailToVerifyWithInvalidBlindedToken) {
  // Arrange
  DLEQProof dleq_proof(kDLEQProofBase64);

  // Act

  // Assert
  EXPECT_FALSE(dleq_proof.Verify(GetInvalidBlindedTokenForTesting(),
                                 GetSignedTokenForTesting(),
                                 GetPublicKeyForTesting()));
}

TEST_F(HnsAdsDLEQProofTest, FailToVerifyWithInvalidSignedToken) {
  // Arrange
  DLEQProof dleq_proof(kDLEQProofBase64);

  // Act

  // Assert
  EXPECT_FALSE(dleq_proof.Verify(GetBlindedTokenForTesting(),
                                 GetInvalidSignedTokenForTesting(),
                                 GetPublicKeyForTesting()));
}

TEST_F(HnsAdsDLEQProofTest, FailToVerifyWithMismatchingPublicKey) {
  // Arrange
  DLEQProof dleq_proof(kDLEQProofBase64);

  // Act

  // AssertGetMismatchingPublicKeyForTesting
  EXPECT_FALSE(dleq_proof.Verify(GetBlindedTokenForTesting(),
                                 GetSignedTokenForTesting(),
                                 GetMismatchingPublicKeyForTesting()));
}

TEST_F(HnsAdsDLEQProofTest, FailToVerifyWithInvalidPublicKey) {
  // Arrange
  DLEQProof dleq_proof(kDLEQProofBase64);

  // Act

  // Assert
  EXPECT_FALSE(dleq_proof.Verify(GetBlindedTokenForTesting(),
                                 GetSignedTokenForTesting(),
                                 GetInvalidPublicKeyForTesting()));
}

TEST_F(HnsAdsDLEQProofTest, IsEqual) {
  // Arrange
  const DLEQProof dleq_proof(kDLEQProofBase64);

  // Act

  // Assert
  EXPECT_EQ(dleq_proof, dleq_proof);
}

TEST_F(HnsAdsDLEQProofTest, IsEqualWhenUninitialized) {
  // Arrange
  const DLEQProof dleq_proof;

  // Act

  // Assert
  EXPECT_EQ(dleq_proof, dleq_proof);
}

TEST_F(HnsAdsDLEQProofTest, IsEmptyBase64Equal) {
  // Arrange
  const DLEQProof dleq_proof("");

  // Act

  // Assert
  EXPECT_EQ(dleq_proof, dleq_proof);
}

TEST_F(HnsAdsDLEQProofTest, IsInvalidBase64Equal) {
  // Arrange
  const DLEQProof dleq_proof(kInvalidBase64);

  // Act

  // Assert
  EXPECT_EQ(dleq_proof, dleq_proof);
}

TEST_F(HnsAdsDLEQProofTest, IsNotEqual) {
  // Arrange
  const DLEQProof dleq_proof(kDLEQProofBase64);

  // Act

  // Assert
  const DLEQProof different_dleq_proof(kInvalidBase64);
  EXPECT_NE(different_dleq_proof, dleq_proof);
}

TEST_F(HnsAdsDLEQProofTest, OutputStream) {
  // Arrange
  const DLEQProof dleq_proof(kDLEQProofBase64);

  // Act
  std::stringstream ss;
  ss << dleq_proof;

  // Assert
  EXPECT_EQ(kDLEQProofBase64, ss.str());
}

TEST_F(HnsAdsDLEQProofTest, OutputStreamWhenUninitialized) {
  // Arrange
  const DLEQProof dleq_proof;

  // Act
  std::stringstream ss;
  ss << dleq_proof;

  // Assert
  EXPECT_TRUE(ss.str().empty());
}

}  // namespace hns_ads::cbr
