/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_WALLET_BROWSER_ETH_GAS_UTILS_H_
#define HNS_COMPONENTS_HNS_WALLET_BROWSER_ETH_GAS_UTILS_H_

#include <string>
#include <vector>

#include "hns/components/hns_wallet/common/hns_wallet_types.h"

namespace hns_wallet {

namespace eth {

absl::optional<uint256_t> ScaleBaseFeePerGas(const std::string& value);

bool GetSuggested1559Fees(const std::vector<std::string>& base_fee_per_gas,
                          const std::vector<double>& gas_used_ratio,
                          const std::string& oldest_block,
                          const std::vector<std::vector<std::string>>& reward,
                          uint256_t* low_priority_fee,
                          uint256_t* avg_priority_fee,
                          uint256_t* high_priority_fee,
                          uint256_t* suggested_base_fee_per_gas);

}  // namespace eth

}  // namespace hns_wallet

#endif  // HNS_COMPONENTS_HNS_WALLET_BROWSER_ETH_GAS_UTILS_H_
