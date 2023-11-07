/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_LEGACY_MIGRATION_REWARDS_LEGACY_REWARDS_MIGRATION_TRANSACTION_UTIL_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_LEGACY_MIGRATION_REWARDS_LEGACY_REWARDS_MIGRATION_TRANSACTION_UTIL_H_

#include "hns/components/hns_ads/core/internal/account/tokens/payment_tokens/payment_token_info.h"
#include "hns/components/hns_ads/core/internal/account/transactions/transaction_info.h"
#include "hns/components/hns_ads/core/internal/legacy_migration/rewards/payment_info.h"
#include "third_party/abseil-cpp/absl/types/optional.h"

namespace hns_ads::rewards {

TransactionList GetAllUnreconciledTransactions(
    const TransactionList& transactions,
    const PaymentTokenList& payment_tokens);

absl::optional<TransactionList>
BuildTransactionsForReconciledTransactionsThisMonth(
    const PaymentList& payments);

absl::optional<TransactionInfo>
BuildTransactionForReconciledTransactionsLastMonth(const PaymentList& payments);

}  // namespace hns_ads::rewards

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_LEGACY_MIGRATION_REWARDS_LEGACY_REWARDS_MIGRATION_TRANSACTION_UTIL_H_
