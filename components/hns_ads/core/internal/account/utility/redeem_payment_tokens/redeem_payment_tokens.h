/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ACCOUNT_UTILITY_REDEEM_PAYMENT_TOKENS_REDEEM_PAYMENT_TOKENS_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ACCOUNT_UTILITY_REDEEM_PAYMENT_TOKENS_REDEEM_PAYMENT_TOKENS_H_

#include "base/check_op.h"
#include "base/memory/raw_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/values.h"
#include "hns/components/hns_ads/core/internal/account/tokens/payment_tokens/payment_token_info.h"
#include "hns/components/hns_ads/core/internal/account/utility/redeem_payment_tokens/redeem_payment_tokens_delegate.h"
#include "hns/components/hns_ads/core/internal/account/wallet/wallet_info.h"
#include "hns/components/hns_ads/core/internal/common/timer/backoff_timer.h"
#include "hns/components/hns_ads/core/internal/common/timer/timer.h"
#include "hns/components/hns_ads/core/mojom/hns_ads.mojom-forward.h"

namespace hns_ads {

class RedeemPaymentTokens final {
 public:
  RedeemPaymentTokens();

  RedeemPaymentTokens(const RedeemPaymentTokens&) = delete;
  RedeemPaymentTokens& operator=(const RedeemPaymentTokens&) = delete;

  RedeemPaymentTokens(RedeemPaymentTokens&&) noexcept = delete;
  RedeemPaymentTokens& operator=(RedeemPaymentTokens&&) noexcept = delete;

  ~RedeemPaymentTokens();

  void SetDelegate(RedeemPaymentTokensDelegate* delegate) {
    CHECK_EQ(delegate_, nullptr);
    delegate_ = delegate;
  }

  void MaybeRedeemAfterDelay(const WalletInfo& wallet);

 private:
  void Redeem();
  void BuildRedeemPaymentTokensUserDataCallback(base::Value::Dict user_data);
  void RedeemCallback(const PaymentTokenList& payment_tokens,
                      const mojom::UrlResponseInfo& url_response);

  void SuccessfullyRedeemed(const PaymentTokenList& payment_tokens);
  void FailedToRedeem(bool should_retry);

  void ScheduleNextRedemption();

  void Retry();
  void RetryCallback();
  void StopRetrying();

  raw_ptr<RedeemPaymentTokensDelegate> delegate_ = nullptr;

  WalletInfo wallet_;

  bool is_processing_ = false;

  Timer timer_;
  BackoffTimer retry_timer_;

  base::WeakPtrFactory<RedeemPaymentTokens> weak_factory_{this};
};

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ACCOUNT_UTILITY_REDEEM_PAYMENT_TOKENS_REDEEM_PAYMENT_TOKENS_H_
