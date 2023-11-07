/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ACCOUNT_UTILITY_REDEEM_CONFIRMATION_REDEEM_CONFIRMATION_FACTORY_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ACCOUNT_UTILITY_REDEEM_CONFIRMATION_REDEEM_CONFIRMATION_FACTORY_H_

#include "base/memory/weak_ptr.h"
#include "hns/components/hns_ads/core/internal/account/utility/redeem_confirmation/redeem_confirmation_delegate.h"

namespace hns_ads {

struct ConfirmationInfo;

class RedeemConfirmationFactory final {
 public:
  static void BuildAndRedeemConfirmation(
      base::WeakPtr<RedeemConfirmationDelegate> delegate,
      const ConfirmationInfo& confirmation);
};

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ACCOUNT_UTILITY_REDEEM_CONFIRMATION_REDEEM_CONFIRMATION_FACTORY_H_
