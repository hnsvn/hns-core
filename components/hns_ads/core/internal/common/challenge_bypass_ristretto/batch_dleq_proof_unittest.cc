/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/common/challenge_bypass_ristretto/batch_dleq_proof.h"

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
#include "hns/components/hns_ads/core/internal/common/challenge_bypass_ristretto/token.h"
#include "hns/components/hns_ads/core/internal/common/challenge_bypass_ristretto/token_unittest_util.h"
#include "hns/components/hns_ads/core/internal/common/challenge_bypass_ristretto/unblinded_token.h"
#include "hns/components/hns_ads/core/internal/common/challenge_bypass_ristretto/unblinded_token_unittest_util.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_base.h"
#include "third_party/abseil-cpp/absl/types/optional.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads::cbr {

class HnsAdsBatchDLEQProofTest : public UnitTestBase {};

TEST_F(HnsAdsBatchDLEQProofTest, FailToInitialize) {
  // Arrange
  const BatchDLEQProof batch_dleq_proof;

  // Act

  // Assert
  EXPECT_FALSE(batch_dleq_proof.has_value());
}

TEST_F(HnsAdsBatchDLEQProofTest, FailToInitializeWithEmptyBase64) {
  // Arrange
  const BatchDLEQProof batch_dleq_proof("");

  // Act

  // Assert
  EXPECT_FALSE(batch_dleq_proof.has_value());
}

TEST_F(HnsAdsBatchDLEQProofTest, FailToInitializeWithInvalidBase64) {
  // Arrange
  const BatchDLEQProof batch_dleq_proof(kInvalidBase64);

  // Act

  // Assert
  EXPECT_FALSE(batch_dleq_proof.has_value());
}

TEST_F(HnsAdsBatchDLEQProofTest, FailToInitializeWithInvalidBlindedTokens) {
  // Arrange
  const BatchDLEQProof batch_dleq_proof(GetInvalidBlindedTokensForTesting(),
                                        GetSignedTokensForTesting(),
                                        GetSigningKeyForTesting());

  // Act

  // Assert
  EXPECT_FALSE(batch_dleq_proof.has_value());
}

TEST_F(HnsAdsBatchDLEQProofTest, FailToInitializeWithInvalidSignedTokens) {
  // Arrange
  const BatchDLEQProof batch_dleq_proof(GetBlindedTokensForTesting(),
                                        GetInvalidSignedTokensForTesting(),
                                        GetSigningKeyForTesting());

  // Act

  // Assert
  EXPECT_FALSE(batch_dleq_proof.has_value());
}

TEST_F(HnsAdsBatchDLEQProofTest, FailToInitializeWithInvalidSigningKey) {
  // Arrange
  const BatchDLEQProof batch_dleq_proof(GetBlindedTokensForTesting(),
                                        GetSignedTokensForTesting(),
                                        GetInvalidSigningKeyForTesting());

  // Act

  // Assert
  EXPECT_FALSE(batch_dleq_proof.has_value());
}

TEST_F(HnsAdsBatchDLEQProofTest, DecodeBase64) {
  // Arrange

  // Act
  const BatchDLEQProof batch_dleq_proof =
      BatchDLEQProof::DecodeBase64(kBatchDLEQProofBase64);

  // Assert
  EXPECT_TRUE(batch_dleq_proof.has_value());
}

TEST_F(HnsAdsBatchDLEQProofTest, FailToDecodeEmptyBase64) {
  // Arrange

  // Act
  const BatchDLEQProof batch_dleq_proof = BatchDLEQProof::DecodeBase64("");

  // Assert
  EXPECT_FALSE(batch_dleq_proof.has_value());
}

TEST_F(HnsAdsBatchDLEQProofTest, FailToDecodeInvalidBase64) {
  // Arrange

  // Act
  const BatchDLEQProof batch_dleq_proof =
      BatchDLEQProof::DecodeBase64(kInvalidBase64);

  // Assert
  EXPECT_FALSE(batch_dleq_proof.has_value());
}

TEST_F(HnsAdsBatchDLEQProofTest, EncodeBase64) {
  // Arrange
  const BatchDLEQProof batch_dleq_proof(kBatchDLEQProofBase64);

  // Act

  // Assert
  EXPECT_EQ(kBatchDLEQProofBase64, batch_dleq_proof.EncodeBase64());
}

TEST_F(HnsAdsBatchDLEQProofTest, FailToEncodeBase64WhenUninitialized) {
  // Arrange
  const BatchDLEQProof batch_dleq_proof;

  // Act

  // Assert
  EXPECT_FALSE(batch_dleq_proof.EncodeBase64());
}

TEST_F(HnsAdsBatchDLEQProofTest, Verify) {
  // Arrange
  BatchDLEQProof batch_dleq_proof(kBatchDLEQProofBase64);

  // Act

  // Assert
  EXPECT_TRUE(batch_dleq_proof.Verify(GetBlindedTokensForTesting(),
                                      GetSignedTokensForTesting(),
                                      GetPublicKeyForTesting()));
}

TEST_F(HnsAdsBatchDLEQProofTest, FailToVerifyWhenUninitialized) {
  // Arrange
  BatchDLEQProof batch_dleq_proof;

  // Act

  // Assert
  EXPECT_FALSE(batch_dleq_proof.Verify(GetBlindedTokensForTesting(),
                                       GetSignedTokensForTesting(),
                                       GetPublicKeyForTesting()));
}

TEST_F(HnsAdsBatchDLEQProofTest, FailToVerifyWithInvalidBlindedTokens) {
  // Arrange
  BatchDLEQProof batch_dleq_proof(kBatchDLEQProofBase64);

  // Act

  // Assert
  EXPECT_FALSE(batch_dleq_proof.Verify(GetInvalidBlindedTokensForTesting(),
                                       GetSignedTokensForTesting(),
                                       GetPublicKeyForTesting()));
}

TEST_F(HnsAdsBatchDLEQProofTest, FailToVerifyWithInvalidSignedTokens) {
  // Arrange
  BatchDLEQProof batch_dleq_proof(kBatchDLEQProofBase64);

  // Act

  // Assert
  EXPECT_FALSE(batch_dleq_proof.Verify(GetBlindedTokensForTesting(),
                                       GetInvalidSignedTokensForTesting(),
                                       GetPublicKeyForTesting()));
}

TEST_F(HnsAdsBatchDLEQProofTest, FailToVerifyWithMismatchingPublicKey) {
  // Arrange
  BatchDLEQProof batch_dleq_proof(kBatchDLEQProofBase64);

  // Act

  // Assert
  EXPECT_FALSE(batch_dleq_proof.Verify(GetBlindedTokensForTesting(),
                                       GetSignedTokensForTesting(),
                                       GetMismatchingPublicKeyForTesting()));
}

TEST_F(HnsAdsBatchDLEQProofTest, FailToVerifyWithInvalidPublicKey) {
  // Arrange
  BatchDLEQProof batch_dleq_proof(kBatchDLEQProofBase64);

  // Act

  // Assert
  EXPECT_FALSE(batch_dleq_proof.Verify(GetBlindedTokensForTesting(),
                                       GetSignedTokensForTesting(),
                                       GetInvalidPublicKeyForTesting()));
}

TEST_F(HnsAdsBatchDLEQProofTest, VerifyAndUnblind) {
  // Arrange
  BatchDLEQProof batch_dleq_proof(kBatchDLEQProofBase64);

  // Act

  // Assert
  EXPECT_EQ(GetUnblindedTokensForTesting(),
            batch_dleq_proof.VerifyAndUnblind(
                GetTokensForTesting(), GetBlindedTokensForTesting(),
                GetSignedTokensForTesting(), GetPublicKeyForTesting()));
}

TEST_F(HnsAdsBatchDLEQProofTest, FailToVerifyAndUnblindWhenUninitialized) {
  // Arrange
  BatchDLEQProof batch_dleq_proof;

  // Act

  // Assert
  ASSERT_FALSE(batch_dleq_proof.VerifyAndUnblind(
      GetTokensForTesting(), GetBlindedTokensForTesting(),
      GetSignedTokensForTesting(), GetPublicKeyForTesting()));
}

TEST_F(HnsAdsBatchDLEQProofTest, FailToVerifyAndUnblindWithInvalidTokens) {
  // Arrange
  BatchDLEQProof batch_dleq_proof(kBatchDLEQProofBase64);

  // Act

  // Assert
  EXPECT_FALSE(batch_dleq_proof.VerifyAndUnblind(
      GetInvalidTokensForTesting(), GetBlindedTokensForTesting(),
      GetSignedTokensForTesting(), GetPublicKeyForTesting()));
}

TEST_F(HnsAdsBatchDLEQProofTest,
       FailToVerifyAndUnblindWithInvalidBlindedTokens) {
  // Arrange
  BatchDLEQProof batch_dleq_proof(kBatchDLEQProofBase64);

  // Act

  // Assert
  EXPECT_FALSE(batch_dleq_proof.VerifyAndUnblind(
      GetTokensForTesting(), GetInvalidBlindedTokensForTesting(),
      GetSignedTokensForTesting(), GetPublicKeyForTesting()));
}

TEST_F(HnsAdsBatchDLEQProofTest,
       FailToVerifyAndUnblindWithInvalidSignedTokens) {
  // Arrange
  BatchDLEQProof batch_dleq_proof(kBatchDLEQProofBase64);

  // Act

  // Assert
  EXPECT_FALSE(batch_dleq_proof.VerifyAndUnblind(
      GetTokensForTesting(), GetBlindedTokensForTesting(),
      GetInvalidSignedTokensForTesting(), GetPublicKeyForTesting()));
}

TEST_F(HnsAdsBatchDLEQProofTest,
       FailToVerifyAndUnblindWithMismatchingPublicKey) {
  // Arrange
  BatchDLEQProof batch_dleq_proof(kBatchDLEQProofBase64);

  // Act

  // Assert
  EXPECT_FALSE(batch_dleq_proof.VerifyAndUnblind(
      GetTokensForTesting(), GetBlindedTokensForTesting(),
      GetSignedTokensForTesting(), GetMismatchingPublicKeyForTesting()));
}

TEST_F(HnsAdsBatchDLEQProofTest, FailToVerifyAndUnblindWithInvalidPublicKey) {
  // Arrange
  BatchDLEQProof batch_dleq_proof(kBatchDLEQProofBase64);

  // Act

  // Assert
  EXPECT_FALSE(batch_dleq_proof.VerifyAndUnblind(
      GetTokensForTesting(), GetBlindedTokensForTesting(),
      GetSignedTokensForTesting(), GetInvalidPublicKeyForTesting()));
}

TEST_F(HnsAdsBatchDLEQProofTest, IsEqual) {
  // Arrange
  const BatchDLEQProof batch_dleq_proof(kBatchDLEQProofBase64);

  // Act

  // Assert
  EXPECT_EQ(batch_dleq_proof, batch_dleq_proof);
}

TEST_F(HnsAdsBatchDLEQProofTest, IsEqualWhenUninitialized) {
  // Arrange
  const BatchDLEQProof batch_dleq_proof;

  // Act

  // Assert
  EXPECT_EQ(batch_dleq_proof, batch_dleq_proof);
}

TEST_F(HnsAdsBatchDLEQProofTest, IsEmptyBase64Equal) {
  // Arrange
  const BatchDLEQProof batch_dleq_proof("");

  // Act

  // Assert
  EXPECT_EQ(batch_dleq_proof, batch_dleq_proof);
}

TEST_F(HnsAdsBatchDLEQProofTest, IsInvalidBase64Equal) {
  // Arrange
  const BatchDLEQProof batch_dleq_proof(kInvalidBase64);

  // Act

  // Assert
  EXPECT_EQ(batch_dleq_proof, batch_dleq_proof);
}

TEST_F(HnsAdsBatchDLEQProofTest, IsNotEqual) {
  // Arrange
  const BatchDLEQProof batch_dleq_proof(kBatchDLEQProofBase64);

  // Act

  // Assert
  const BatchDLEQProof different_batch_dleq_proof(kInvalidBase64);
  EXPECT_NE(different_batch_dleq_proof, batch_dleq_proof);
}

TEST_F(HnsAdsBatchDLEQProofTest, OutputStream) {
  // Arrange
  const BatchDLEQProof batch_dleq_proof(kBatchDLEQProofBase64);

  // Act
  std::stringstream ss;
  ss << batch_dleq_proof;

  // Assert
  EXPECT_EQ(kBatchDLEQProofBase64, ss.str());
}

TEST_F(HnsAdsBatchDLEQProofTest, OutputStreamWhenUninitialized) {
  // Arrange
  const BatchDLEQProof batch_dleq_proof;

  // Act
  std::stringstream ss;
  ss << batch_dleq_proof;

  // Assert
  EXPECT_TRUE(ss.str().empty());
}

}  // namespace hns_ads::cbr
