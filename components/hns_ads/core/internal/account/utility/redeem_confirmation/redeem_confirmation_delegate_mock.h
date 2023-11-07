/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ACCOUNT_UTILITY_REDEEM_CONFIRMATION_REDEEM_CONFIRMATION_DELEGATE_MOCK_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ACCOUNT_UTILITY_REDEEM_CONFIRMATION_REDEEM_CONFIRMATION_DELEGATE_MOCK_H_

#include "hns/components/hns_ads/core/internal/account/utility/redeem_confirmation/redeem_confirmation_delegate.h"
#include "testing/gmock/include/gmock/gmock.h"

namespace hns_ads {

class RedeemConfirmationDelegateMock : public RedeemConfirmationDelegate {
 public:
  RedeemConfirmationDelegateMock();

  RedeemConfirmationDelegateMock(const RedeemConfirmationDelegateMock&) =
      delete;
  RedeemConfirmationDelegateMock& operator=(
      const RedeemConfirmationDelegateMock&) = delete;

  RedeemConfirmationDelegateMock(RedeemConfirmationDelegateMock&&) noexcept =
      delete;
  RedeemConfirmationDelegateMock& operator=(
      RedeemConfirmationDelegateMock&&) noexcept = delete;

  ~RedeemConfirmationDelegateMock() override;

  MOCK_METHOD(void,
              OnDidRedeemConfirmation,
              (const ConfirmationInfo& confirmation));

  MOCK_METHOD(void,
              OnFailedToRedeemConfirmation,
              (const ConfirmationInfo& confirmation, bool should_retry));
};

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ACCOUNT_UTILITY_REDEEM_CONFIRMATION_REDEEM_CONFIRMATION_DELEGATE_MOCK_H_
