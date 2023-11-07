/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/account/deposits/cash_deposit.h"

#include <utility>

#include "base/functional/bind.h"
#include "base/functional/callback.h"
#include "hns/components/hns_ads/core/internal/account/deposits/deposit_info.h"
#include "hns/components/hns_ads/core/internal/account/deposits/deposits_database_table.h"
#include "third_party/abseil-cpp/absl/types/optional.h"

namespace hns_ads {

void CashDeposit::GetValue(const std::string& creative_instance_id,
                           GetDepositCallback callback) {
  const database::table::Deposits database_table;
  database_table.GetForCreativeInstanceId(
      creative_instance_id,
      base::BindOnce(
          [](GetDepositCallback callback, const bool success,
             const absl::optional<DepositInfo>& deposit) {
            if (!success) {
              return std::move(callback).Run(/*success */ false,
                                             /*value*/ 0.0);
            }

            if (!deposit) {
              return std::move(callback).Run(/*success */ false,
                                             /*value*/ 0.0);
            }

            std::move(callback).Run(/*success*/ true, deposit->value);
          },
          std::move(callback)));
}

}  // namespace hns_ads
