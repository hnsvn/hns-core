/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ACCOUNT_UTILITY_REDEEM_CONFIRMATION_NON_REWARD_REDEEM_NON_REWARD_CONFIRMATION_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ACCOUNT_UTILITY_REDEEM_CONFIRMATION_NON_REWARD_REDEEM_NON_REWARD_CONFIRMATION_H_

#include "base/memory/weak_ptr.h"
#include "hns/components/hns_ads/core/internal/account/utility/redeem_confirmation/redeem_confirmation_delegate.h"
#include "hns/components/hns_ads/core/mojom/hns_ads.mojom-forward.h"

namespace hns_ads {

struct ConfirmationInfo;

class RedeemNonRewardConfirmation final {
 public:
  RedeemNonRewardConfirmation(const RedeemNonRewardConfirmation&) = delete;
  RedeemNonRewardConfirmation& operator=(const RedeemNonRewardConfirmation&) =
      delete;

  RedeemNonRewardConfirmation(RedeemNonRewardConfirmation&&) noexcept;
  RedeemNonRewardConfirmation& operator=(
      RedeemNonRewardConfirmation&&) noexcept;

  ~RedeemNonRewardConfirmation();

  static void CreateAndRedeem(
      base::WeakPtr<RedeemConfirmationDelegate> delegate,
      const ConfirmationInfo& confirmation);

 private:
  explicit RedeemNonRewardConfirmation(
      base::WeakPtr<RedeemConfirmationDelegate> delegate);

  static void Redeem(RedeemNonRewardConfirmation redeem_confirmation,
                     const ConfirmationInfo& confirmation);

  static void CreateConfirmation(
      RedeemNonRewardConfirmation redeem_confirmation,
      const ConfirmationInfo& confirmation);
  static void CreateConfirmationCallback(
      RedeemNonRewardConfirmation redeem_confirmation,
      const ConfirmationInfo& confirmation,
      const mojom::UrlResponseInfo& url_response);

  void SuccessfullyRedeemedConfirmation(const ConfirmationInfo& confirmation);
  void FailedToRedeemConfirmation(const ConfirmationInfo& confirmation,
                                  bool should_retry);

  base::WeakPtr<RedeemConfirmationDelegate> delegate_ = nullptr;
};

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ACCOUNT_UTILITY_REDEEM_CONFIRMATION_NON_REWARD_REDEEM_NON_REWARD_CONFIRMATION_H_
