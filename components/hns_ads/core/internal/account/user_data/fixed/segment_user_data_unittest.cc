/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/account/user_data/fixed/segment_user_data.h"

#include "base/test/values_test_util.h"
#include "hns/components/hns_ads/core/internal/account/transactions/transaction_info.h"
#include "hns/components/hns_ads/core/internal/account/transactions/transactions_unittest_util.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_base.h"
#include "hns/components/hns_ads/core/internal/settings/settings_unittest_util.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads {

class HnsAdsSegmentUserDataTest : public UnitTestBase {};

TEST_F(HnsAdsSegmentUserDataTest, BuildSegmentUserDataForRewardsUser) {
  // Arrange
  const TransactionInfo transaction = BuildUnreconciledTransactionForTesting(
      /*value*/ 0.01, ConfirmationType::kViewed,
      /*should_use_random_uuids*/ false);

  // Act

  // Assert
  EXPECT_EQ(base::test::ParseJsonDict(R"({"segment":"untargeted"})"),
            BuildSegmentUserData(transaction));
}

TEST_F(HnsAdsSegmentUserDataTest,
       DoNotBuildSegmentUserDataForNonRewardsUser) {
  // Arrange
  DisableHnsRewardsForTesting();

  const TransactionInfo transaction = BuildUnreconciledTransactionForTesting(
      /*value*/ 0.01, ConfirmationType::kViewed,
      /*should_use_random_uuids*/ false);

  // Act

  // Assert
  EXPECT_TRUE(BuildSegmentUserData(transaction).empty());
}

TEST_F(HnsAdsSegmentUserDataTest, DoNotBuildSegmentUserDataIfEmptySegment) {
  // Arrange
  const TransactionInfo transaction;

  // Act

  // Assert
  EXPECT_TRUE(BuildSegmentUserData(transaction).empty());
}

}  // namespace hns_ads
