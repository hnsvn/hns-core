/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/account/confirmations/reward/reward_credential_json_writer.h"

#include "hns/components/hns_ads/core/internal/account/confirmations/confirmation_info.h"
#include "hns/components/hns_ads/core/internal/account/confirmations/reward/reward_confirmation_util.h"
#include "hns/components/hns_ads/core/internal/account/confirmations/reward/reward_info.h"
#include "hns/components/hns_ads/core/internal/account/confirmations/reward/reward_unittest_util.h"
#include "hns/components/hns_ads/core/internal/account/tokens/confirmation_tokens/confirmation_tokens_unittest_util.h"
#include "hns/components/hns_ads/core/internal/account/tokens/token_generator_mock.h"
#include "hns/components/hns_ads/core/internal/account/tokens/token_generator_unittest_util.h"
#include "hns/components/hns_ads/core/internal/account/transactions/transactions_unittest_util.h"
#include "hns/components/hns_ads/core/internal/account/user_data/user_data_info.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_base.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads {

class HnsAdsRewardCredentialJsonWriterTest : public UnitTestBase {
 protected:
  ::testing::NiceMock<TokenGeneratorMock> token_generator_mock_;
};

TEST_F(HnsAdsRewardCredentialJsonWriterTest, WriteRewardCredential) {
  // Arrange
  MockTokenGenerator(token_generator_mock_, /*count*/ 1);

  SetConfirmationTokensForTesting(/*count*/ 1);

  const TransactionInfo transaction = BuildUnreconciledTransactionForTesting(
      /*value*/ 0.01, ConfirmationType::kViewed,
      /*should_use_random_uuids*/ false);
  const absl::optional<ConfirmationInfo> confirmation = BuildRewardConfirmation(
      &token_generator_mock_, transaction, /*user_data*/ {});
  ASSERT_TRUE(confirmation);

  // Act

  // Assert
  EXPECT_EQ(
      R"({"signature":"XsaQ/XqKiWfeTCjFDhkyldsx0086qu6tjgJDCKo+f7kA0eA+mdf3Ae+BjPcDDQ8JfVbVQkI5ub394qdTmE2bRw==","t":"PLowz2WF2eGD5zfwZjk9p76HXBLDKMq/3EAZHeG/fE2XGQ48jyte+Ve50ZlasOuYL5mwA8CU2aFMlJrt3DDgCw=="})",
      json::writer::WriteRewardCredential(
          BuildRewardForTesting(*confirmation),
          /*payload*/ "definition: the weight of a payload"));
}

}  // namespace hns_ads
