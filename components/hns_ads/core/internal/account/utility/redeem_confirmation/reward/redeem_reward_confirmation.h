/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ACCOUNT_UTILITY_REDEEM_CONFIRMATION_REWARD_REDEEM_REWARD_CONFIRMATION_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ACCOUNT_UTILITY_REDEEM_CONFIRMATION_REWARD_REDEEM_REWARD_CONFIRMATION_H_

#include "base/memory/weak_ptr.h"
#include "hns/components/hns_ads/core/internal/account/utility/redeem_confirmation/redeem_confirmation_delegate.h"
#include "hns/components/hns_ads/core/mojom/hns_ads.mojom-forward.h"

namespace hns_ads {

struct ConfirmationInfo;

class RedeemRewardConfirmation final {
 public:
  RedeemRewardConfirmation(const RedeemRewardConfirmation&) = delete;
  RedeemRewardConfirmation& operator=(const RedeemRewardConfirmation&) = delete;

  RedeemRewardConfirmation(RedeemRewardConfirmation&&) noexcept;
  RedeemRewardConfirmation& operator=(RedeemRewardConfirmation&&) noexcept;

  ~RedeemRewardConfirmation();

  static void CreateAndRedeem(
      base::WeakPtr<RedeemConfirmationDelegate> delegate,
      const ConfirmationInfo& confirmation);

 private:
  explicit RedeemRewardConfirmation(
      base::WeakPtr<RedeemConfirmationDelegate> delegate);

  static void Redeem(RedeemRewardConfirmation redeem_confirmation,
                     const ConfirmationInfo& confirmation);

  static void CreateConfirmation(RedeemRewardConfirmation redeem_confirmation,
                                 const ConfirmationInfo& confirmation);
  static void CreateConfirmationCallback(
      RedeemRewardConfirmation redeem_confirmation,
      const ConfirmationInfo& confirmation,
      const mojom::UrlResponseInfo& url_response);

  static void FetchPaymentToken(RedeemRewardConfirmation redeem_confirmation,
                                const ConfirmationInfo& confirmation);
  static void FetchPaymentTokenCallback(
      RedeemRewardConfirmation redeem_confirmation,
      const ConfirmationInfo& confirmation,
      const mojom::UrlResponseInfo& url_response);

  void SuccessfullyRedeemedConfirmation(const ConfirmationInfo& confirmation);
  void FailedToRedeemConfirmation(const ConfirmationInfo& confirmation,
                                  bool should_retry);

  base::WeakPtr<RedeemConfirmationDelegate> delegate_ = nullptr;
};

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ACCOUNT_UTILITY_REDEEM_CONFIRMATION_REWARD_REDEEM_REWARD_CONFIRMATION_H_
