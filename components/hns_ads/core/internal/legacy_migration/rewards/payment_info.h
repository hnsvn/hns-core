/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_LEGACY_MIGRATION_REWARDS_PAYMENT_INFO_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_LEGACY_MIGRATION_REWARDS_PAYMENT_INFO_H_

#include <string>
#include <vector>

namespace hns_ads::rewards {

struct PaymentInfo final {
  double balance = 0.0;
  std::string month;
  int transaction_count = 0;
};

using PaymentList = std::vector<PaymentInfo>;

}  // namespace hns_ads::rewards

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_LEGACY_MIGRATION_REWARDS_PAYMENT_INFO_H_
