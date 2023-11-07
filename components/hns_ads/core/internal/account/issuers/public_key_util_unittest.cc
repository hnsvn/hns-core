/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/account/issuers/public_key_util.h"

#include "hns/components/hns_ads/core/internal/account/issuers/issuer_info.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_base.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads {

namespace {

constexpr char kPublicKey[] = "bCKwI6tx5LWrZKxWbW5CxaVIGe2N0qGYLfFE+38urCg=";
constexpr char kMissingPublicKey[] =
    "QnShwT9vRebch3WDu28nqlTaNCU5MaOF1n4VV4Q3K1g==";

}  // namespace

class HnsAdsPublicKeyUtilTest : public UnitTestBase {};

TEST_F(HnsAdsPublicKeyUtilTest, PublicKeyExists) {
  // Arrange
  IssuerInfo issuer;
  issuer.public_keys.insert({kPublicKey, /*associated_value*/ 0.1});

  // Act

  // Assert
  EXPECT_TRUE(PublicKeyExists(issuer, kPublicKey));
}

TEST_F(HnsAdsPublicKeyUtilTest, PublicKeyDoesNotExist) {
  // Arrange
  IssuerInfo issuer;
  issuer.public_keys.insert({kPublicKey, /*associated_value*/ 0.1});

  // Act

  // Assert
  EXPECT_FALSE(PublicKeyExists(issuer, kMissingPublicKey));
}

TEST_F(HnsAdsPublicKeyUtilTest, NoPublicKeys) {
  // Arrange
  const IssuerInfo issuer;

  // Act

  // Assert
  EXPECT_FALSE(PublicKeyExists(issuer, kPublicKey));
}

}  // namespace hns_ads
