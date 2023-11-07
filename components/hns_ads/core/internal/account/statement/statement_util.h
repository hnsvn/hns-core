/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ACCOUNT_STATEMENT_STATEMENT_UTIL_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ACCOUNT_STATEMENT_STATEMENT_UTIL_H_

#include <string>
#include <utility>

#include "base/containers/flat_map.h"
#include "hns/components/hns_ads/core/internal/account/transactions/transaction_info.h"

namespace base {
class Time;
}  // namespace base

namespace hns_ads {

base::Time GetNextPaymentDate(const TransactionList& transactions);

std::pair</*range_low*/ double, /*range_high*/ double>
GetEstimatedEarningsForThisMonth(const TransactionList& transactions);

std::pair</*range_low*/ double, /*range_high*/ double>
GetEstimatedEarningsForLastMonth(const TransactionList& transactions);

base::flat_map<std::string, int32_t> GetAdTypesReceivedThisMonth(
    const TransactionList& transactions);

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ACCOUNT_STATEMENT_STATEMENT_UTIL_H_
