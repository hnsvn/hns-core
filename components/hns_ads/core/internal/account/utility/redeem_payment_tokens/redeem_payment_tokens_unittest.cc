/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/account/utility/redeem_payment_tokens/redeem_payment_tokens.h"

#include <memory>

#include "hns/components/hns_ads/core/internal/account/tokens/payment_tokens/payment_token_util.h"
#include "hns/components/hns_ads/core/internal/account/tokens/payment_tokens/payment_tokens.h"
#include "hns/components/hns_ads/core/internal/account/tokens/payment_tokens/payment_tokens_unittest_util.h"
#include "hns/components/hns_ads/core/internal/account/utility/redeem_payment_tokens/redeem_payment_tokens_delegate_mock.h"
#include "hns/components/hns_ads/core/internal/account/utility/redeem_payment_tokens/redeem_payment_tokens_unittest_util.h"
#include "hns/components/hns_ads/core/internal/account/utility/redeem_payment_tokens/url_request_builders/redeem_payment_tokens_url_request_builder_util.h"
#include "hns/components/hns_ads/core/internal/account/wallet/wallet_info.h"
#include "hns/components/hns_ads/core/internal/account/wallet/wallet_unittest_constants.h"
#include "hns/components/hns_ads/core/internal/account/wallet/wallet_unittest_util.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_base.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_mock_util.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_pref_util.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_time_util.h"
#include "hns/components/hns_ads/core/public/prefs/pref_names.h"
#include "net/http/http_status_code.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads {

class HnsAdsRedeemPaymentTokensTest : public UnitTestBase {
 protected:
  HnsAdsRedeemPaymentTokensTest()
      : redeem_payment_tokens_(std::make_unique<RedeemPaymentTokens>()) {
    redeem_payment_tokens_->SetDelegate(&redeem_payment_tokens_delegate_mock_);
  }

  std::unique_ptr<RedeemPaymentTokens> redeem_payment_tokens_;
  ::testing::NiceMock<RedeemPaymentTokensDelegateMock>
      redeem_payment_tokens_delegate_mock_;
};

TEST_F(HnsAdsRedeemPaymentTokensTest, RedeemPaymentTokens) {
  // Arrange
  const URLResponseMap url_responses = {
      {BuildRedeemPaymentTokensUrlPath(
           /*payment_id*/ kWalletPaymentId),
       {{net::HTTP_OK, BuildRedeemPaymentTokensUrlResponseBodyForTesting()}}}};
  MockUrlResponses(ads_client_mock_, url_responses);

  SetTimePref(prefs::kNextTokenRedemptionAt, Now());

  SetPaymentTokensForTesting(/*count*/ 1);

  // Act
  EXPECT_CALL(redeem_payment_tokens_delegate_mock_, OnDidRedeemPaymentTokens);

  EXPECT_CALL(redeem_payment_tokens_delegate_mock_,
              OnFailedToRedeemPaymentTokens())
      .Times(0);

  EXPECT_CALL(redeem_payment_tokens_delegate_mock_,
              OnDidScheduleNextPaymentTokenRedemption);

  EXPECT_CALL(redeem_payment_tokens_delegate_mock_,
              OnWillRetryRedeemingPaymentTokens)
      .Times(0);

  EXPECT_CALL(redeem_payment_tokens_delegate_mock_,
              OnDidRetryRedeemingPaymentTokens())
      .Times(0);

  const WalletInfo wallet = GetWalletForTesting();
  redeem_payment_tokens_->MaybeRedeemAfterDelay(wallet);

  FastForwardClockToNextPendingTask();

  // Assert
  EXPECT_TRUE(PaymentTokensIsEmpty());
}

TEST_F(HnsAdsRedeemPaymentTokensTest, RedeemPaymentTokensMultipleTimes) {
  // Arrange
  const URLResponseMap url_responses = {
      {BuildRedeemPaymentTokensUrlPath(
           /*payment_id*/ kWalletPaymentId),
       {{net::HTTP_OK, BuildRedeemPaymentTokensUrlResponseBodyForTesting()},
        {net::HTTP_OK, BuildRedeemPaymentTokensUrlResponseBodyForTesting()}}}};
  MockUrlResponses(ads_client_mock_, url_responses);

  SetTimePref(prefs::kNextTokenRedemptionAt, Now());

  const PaymentTokenList payment_tokens =
      SetPaymentTokensForTesting(/*count*/ 1);

  // Act
  EXPECT_CALL(redeem_payment_tokens_delegate_mock_, OnDidRedeemPaymentTokens)
      .Times(2);

  EXPECT_CALL(redeem_payment_tokens_delegate_mock_,
              OnFailedToRedeemPaymentTokens())
      .Times(0);

  EXPECT_CALL(redeem_payment_tokens_delegate_mock_,
              OnDidScheduleNextPaymentTokenRedemption)
      .Times(2);

  EXPECT_CALL(redeem_payment_tokens_delegate_mock_,
              OnWillRetryRedeemingPaymentTokens)
      .Times(0);

  EXPECT_CALL(redeem_payment_tokens_delegate_mock_,
              OnDidRetryRedeemingPaymentTokens())
      .Times(0);

  const WalletInfo wallet = GetWalletForTesting();
  redeem_payment_tokens_->MaybeRedeemAfterDelay(wallet);

  FastForwardClockToNextPendingTask();

  GetPaymentTokensForTesting().SetTokens(payment_tokens);

  FastForwardClockToNextPendingTask();

  // Assert
  EXPECT_EQ(1U, GetPendingTaskCount());
}

TEST_F(HnsAdsRedeemPaymentTokensTest, ScheduleNextTokenRedemption) {
  // Arrange
  const URLResponseMap url_responses = {
      {BuildRedeemPaymentTokensUrlPath(
           /*payment_id*/ kWalletPaymentId),
       {{net::HTTP_OK, BuildRedeemPaymentTokensUrlResponseBodyForTesting()}}}};
  MockUrlResponses(ads_client_mock_, url_responses);

  SetTimePref(prefs::kNextTokenRedemptionAt, Now());

  SetPaymentTokensForTesting(/*count*/ 1);

  // Act
  EXPECT_CALL(redeem_payment_tokens_delegate_mock_, OnDidRedeemPaymentTokens);

  EXPECT_CALL(redeem_payment_tokens_delegate_mock_,
              OnFailedToRedeemPaymentTokens())
      .Times(0);

  EXPECT_CALL(redeem_payment_tokens_delegate_mock_,
              OnDidScheduleNextPaymentTokenRedemption);

  EXPECT_CALL(redeem_payment_tokens_delegate_mock_,
              OnWillRetryRedeemingPaymentTokens)
      .Times(0);

  EXPECT_CALL(redeem_payment_tokens_delegate_mock_,
              OnDidRetryRedeemingPaymentTokens())
      .Times(0);

  const WalletInfo wallet = GetWalletForTesting();
  redeem_payment_tokens_->MaybeRedeemAfterDelay(wallet);

  FastForwardClockToNextPendingTask();

  // Assert
  EXPECT_TRUE(PaymentTokensIsEmpty());
}

TEST_F(HnsAdsRedeemPaymentTokensTest, NoPaymentTokens) {
  // Arrange
  SetTimePref(prefs::kNextTokenRedemptionAt, Now());

  // Act
  EXPECT_CALL(ads_client_mock_, UrlRequest).Times(0);

  EXPECT_CALL(redeem_payment_tokens_delegate_mock_, OnDidRedeemPaymentTokens)
      .Times(0);

  EXPECT_CALL(redeem_payment_tokens_delegate_mock_,
              OnFailedToRedeemPaymentTokens())
      .Times(0);

  EXPECT_CALL(redeem_payment_tokens_delegate_mock_,
              OnDidScheduleNextPaymentTokenRedemption);

  EXPECT_CALL(redeem_payment_tokens_delegate_mock_,
              OnWillRetryRedeemingPaymentTokens)
      .Times(0);

  EXPECT_CALL(redeem_payment_tokens_delegate_mock_,
              OnDidRetryRedeemingPaymentTokens())
      .Times(0);

  const WalletInfo wallet = GetWalletForTesting();
  redeem_payment_tokens_->MaybeRedeemAfterDelay(wallet);

  FastForwardClockToNextPendingTask();

  // Assert
  EXPECT_TRUE(PaymentTokensIsEmpty());
}

TEST_F(HnsAdsRedeemPaymentTokensTest, Retry) {
  // Arrange
  const URLResponseMap url_responses = {
      {BuildRedeemPaymentTokensUrlPath(
           /*payment_id*/ kWalletPaymentId),
       {{net::HTTP_NOT_FOUND,
         /*response_body*/ net::GetHttpReasonPhrase(net::HTTP_NOT_FOUND)},
        {net::HTTP_OK, BuildRedeemPaymentTokensUrlResponseBodyForTesting()}}}};
  MockUrlResponses(ads_client_mock_, url_responses);

  SetTimePref(prefs::kNextTokenRedemptionAt, Now());

  SetPaymentTokensForTesting(/*count*/ 1);

  // Act
  const ::testing::InSequence seq;

  EXPECT_CALL(redeem_payment_tokens_delegate_mock_,
              OnFailedToRedeemPaymentTokens());

  EXPECT_CALL(redeem_payment_tokens_delegate_mock_,
              OnWillRetryRedeemingPaymentTokens);

  EXPECT_CALL(redeem_payment_tokens_delegate_mock_,
              OnDidRetryRedeemingPaymentTokens());

  EXPECT_CALL(redeem_payment_tokens_delegate_mock_, OnDidRedeemPaymentTokens);

  EXPECT_CALL(redeem_payment_tokens_delegate_mock_,
              OnDidScheduleNextPaymentTokenRedemption);

  const WalletInfo wallet = GetWalletForTesting();
  redeem_payment_tokens_->MaybeRedeemAfterDelay(wallet);

  FastForwardClockToNextPendingTask();
  FastForwardClockToNextPendingTask();

  // Assert
  EXPECT_TRUE(PaymentTokensIsEmpty());
}

}  // namespace hns_ads
