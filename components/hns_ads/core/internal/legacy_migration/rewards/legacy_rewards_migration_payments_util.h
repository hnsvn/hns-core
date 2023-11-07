/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_LEGACY_MIGRATION_REWARDS_LEGACY_REWARDS_MIGRATION_PAYMENTS_UTIL_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_LEGACY_MIGRATION_REWARDS_LEGACY_REWARDS_MIGRATION_PAYMENTS_UTIL_H_

#include "hns/components/hns_ads/core/internal/legacy_migration/rewards/payment_info.h"
#include "third_party/abseil-cpp/absl/types/optional.h"

namespace hns_ads::rewards {

absl::optional<PaymentInfo> GetPaymentForThisMonth(const PaymentList& payments);
absl::optional<PaymentInfo> GetPaymentForLastMonth(const PaymentList& payments);

}  // namespace hns_ads::rewards

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_LEGACY_MIGRATION_REWARDS_LEGACY_REWARDS_MIGRATION_PAYMENTS_UTIL_H_
