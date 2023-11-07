/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/common/challenge_bypass_ristretto/verification_key.h"

#include "hns/components/hns_ads/core/internal/common/challenge_bypass_ristretto/verification_key_unittest_util.h"
#include "hns/components/hns_ads/core/internal/common/challenge_bypass_ristretto/verification_signature.h"
#include "hns/components/hns_ads/core/internal/common/challenge_bypass_ristretto/verification_signature_unittest_util.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_base.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads::cbr {

namespace {
constexpr char kMessage[] = "The quick brown fox jumps over the lazy dog";
}  // namespace

class HnsAdsVerificationKeyTest : public UnitTestBase {};

TEST_F(HnsAdsVerificationKeyTest, Sign) {
  // Arrange
  VerificationKey verification_key = GetVerificationKeyForTesting();

  // Act

  // Assert
  EXPECT_EQ(GetVerificationSignatureForTesting(),
            verification_key.Sign(kMessage));
}

TEST_F(HnsAdsVerificationKeyTest, Verify) {
  // Arrange
  VerificationKey verification_key = GetVerificationKeyForTesting();

  // Act
  // Assert
  EXPECT_TRUE(
      verification_key.Verify(GetVerificationSignatureForTesting(), kMessage));
}

TEST_F(HnsAdsVerificationKeyTest,
       FailToVerifyWithInvalidVerificationSignature) {
  // Arrange
  VerificationKey verification_key = GetVerificationKeyForTesting();

  // Act

  // Assert
  EXPECT_FALSE(verification_key.Verify(
      GetInvalidVerificationSignatureForTesting(), kMessage));
}

}  // namespace hns_ads::cbr
