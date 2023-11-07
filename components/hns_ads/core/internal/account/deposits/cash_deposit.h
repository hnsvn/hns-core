/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ACCOUNT_DEPOSITS_CASH_DEPOSIT_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ACCOUNT_DEPOSITS_CASH_DEPOSIT_H_

#include <string>

#include "hns/components/hns_ads/core/internal/account/deposits/deposit_interface.h"

namespace hns_ads {

class CashDeposit final : public DepositInterface {
 public:
  void GetValue(const std::string& creative_instance_id,
                GetDepositCallback callback) override;
};

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ACCOUNT_DEPOSITS_CASH_DEPOSIT_H_
