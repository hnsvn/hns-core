/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/account/utility/redeem_confirmation/non_reward/url_request_builders/create_non_reward_confirmation_url_request_builder_util.h"

#include "hns/components/hns_ads/core/internal/account/transactions/transaction_unittest_constants.h"
#include "testing/gtest/include/gtest/gtest.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads {

TEST(HnsAdsCreateNonRewardConfirmationUrlRequestBuilderUtilTest, GetPath) {
  // Arrange

  // Act

  // Assert
  EXPECT_EQ("/v3/confirmation/8b742869-6e4a-490c-ac31-31b49130098a",
            BuildCreateNonRewardConfirmationUrlPath(kTransactionId));
}

}  // namespace hns_ads
