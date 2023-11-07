/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/account/statement/statement_util.h"

#include <iterator>

#include "base/ranges/algorithm.h"
#include "base/time/time.h"
#include "hns/components/hns_ads/core/internal/account/statement/ads_received_util.h"
#include "hns/components/hns_ads/core/internal/account/statement/earnings_util.h"
#include "hns/components/hns_ads/core/internal/account/statement/next_payment_date_util.h"
#include "hns/components/hns_ads/core/internal/account/statement/statement_feature.h"
#include "hns/components/hns_ads/core/internal/client/ads_client_helper.h"
#include "hns/components/hns_ads/core/internal/common/time/time_util.h"
#include "hns/components/hns_ads/core/public/prefs/pref_names.h"

namespace hns_ads {

namespace {

TransactionList FilterTransactionsForEstimatedEarnings(
    const TransactionList& transactions) {
  TransactionList filtered_transactions;
  base::ranges::copy_if(transactions, std::back_inserter(filtered_transactions),
                        [](const TransactionInfo& transaction) {
                          return transaction.ad_type != AdType::kNewTabPageAd;
                        });
  return filtered_transactions;
}

}  // namespace

base::Time GetNextPaymentDate(const TransactionList& transactions) {
  const base::Time next_token_redemption_at =
      AdsClientHelper::GetInstance()->GetTimePref(
          prefs::kNextTokenRedemptionAt);

  const base::Time next_payment_date =
      CalculateNextPaymentDate(next_token_redemption_at, transactions);

  return next_payment_date;
}

std::pair<double, double> GetEstimatedEarningsForThisMonth(
    const TransactionList& transactions) {
  TransactionList filtered_transactions =
      FilterTransactionsForEstimatedEarnings(transactions);

  const double range_low =
      GetUnreconciledEarnings(filtered_transactions) +
      GetReconciledEarningsForThisMonth(filtered_transactions);

  const double range_high = GetUnreconciledEarnings(transactions) +
                            GetReconciledEarningsForThisMonth(transactions);

  return {range_low * kMinEstimatedEarningsMultiplier.Get(), range_high};
}

std::pair<double, double> GetEstimatedEarningsForLastMonth(
    const TransactionList& transactions) {
  const double range_low = GetReconciledEarningsForLastMonth(
      FilterTransactionsForEstimatedEarnings(transactions));
  const double range_high = GetReconciledEarningsForLastMonth(transactions);

  return {range_low * kMinEstimatedEarningsMultiplier.Get(), range_high};
}

base::flat_map<std::string, int32_t> GetAdTypesReceivedThisMonth(
    const TransactionList& transactions) {
  const base::Time from_time = GetLocalTimeAtBeginningOfThisMonth();
  const base::Time to_time = GetLocalTimeAtEndOfThisMonth();

  return GetAdTypesReceivedForDateRange(transactions, from_time, to_time);
}

}  // namespace hns_ads
