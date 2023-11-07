/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/account/issuers/confirmations_issuer_util.h"

#include "base/uuid.h"
#include "hns/components/hns_ads/core/internal/account/issuers/issuer_info.h"
#include "hns/components/hns_ads/core/internal/account/issuers/issuers_constants.h"
#include "hns/components/hns_ads/core/internal/account/issuers/issuers_info.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_base.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads {

class HnsAdsConfirmationsIssuerUtilTest : public UnitTestBase {};

TEST_F(HnsAdsConfirmationsIssuerUtilTest, IsValid) {
  // Arrange
  IssuerInfo issuer;
  issuer.type = IssuerType::kConfirmations;

  for (int i = 0; i < kMaximumIssuerPublicKeys; i++) {
    issuer.public_keys.insert(
        {/*public_key*/ base::Uuid::GenerateRandomV4().AsLowercaseString(),
         /*associated_value*/ 0.1});
  }

  IssuersInfo issuers;
  issuers.issuers.push_back(issuer);

  // Act

  // Assert
  EXPECT_TRUE(IsConfirmationsIssuerValid(issuers));
}

TEST_F(HnsAdsConfirmationsIssuerUtilTest, IsInvalid) {
  // Arrange
  IssuerInfo issuer;
  issuer.type = IssuerType::kConfirmations;

  for (int i = 0; i < kMaximumIssuerPublicKeys + 1; i++) {
    issuer.public_keys.insert(
        {/*public_key*/ base::Uuid::GenerateRandomV4().AsLowercaseString(),
         /*associated_value*/ 0.1});
  }

  IssuersInfo issuers;
  issuers.issuers.push_back(issuer);

  // Act

  // Assert
  EXPECT_FALSE(IsConfirmationsIssuerValid(issuers));
}

}  // namespace hns_ads
