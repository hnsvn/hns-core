/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ACCOUNT_TRANSACTIONS_TRANSACTION_INFO_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ACCOUNT_TRANSACTIONS_TRANSACTION_INFO_H_

#include <string>
#include <vector>

#include "base/time/time.h"
#include "hns/components/hns_ads/core/public/ad_type.h"
#include "hns/components/hns_ads/core/public/confirmation_type.h"

namespace hns_ads {

struct TransactionInfo final {
  TransactionInfo();

  TransactionInfo(const TransactionInfo&);
  TransactionInfo& operator=(const TransactionInfo&);

  TransactionInfo(TransactionInfo&&) noexcept;
  TransactionInfo& operator=(TransactionInfo&&) noexcept;

  ~TransactionInfo();

  bool operator==(const TransactionInfo&) const;
  bool operator!=(const TransactionInfo&) const;

  [[nodiscard]] bool IsValid() const;

  std::string id;
  base::Time created_at;
  std::string creative_instance_id;
  std::string segment;
  double value = 0.0;
  AdType ad_type = AdType::kUndefined;
  ConfirmationType confirmation_type = ConfirmationType::kUndefined;
  base::Time reconciled_at;
};

using TransactionList = std::vector<TransactionInfo>;

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ACCOUNT_TRANSACTIONS_TRANSACTION_INFO_H_
