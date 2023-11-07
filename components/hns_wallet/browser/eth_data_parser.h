/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_WALLET_BROWSER_ETH_DATA_PARSER_H_
#define HNS_COMPONENTS_HNS_WALLET_BROWSER_ETH_DATA_PARSER_H_

#include <string>
#include <tuple>
#include <vector>

#include "hns/components/hns_wallet/common/hns_wallet.mojom.h"

namespace hns_wallet {

absl::optional<std::tuple<mojom::TransactionType,     // tx_type
                          std::vector<std::string>,   // tx_params
                          std::vector<std::string>>>  // tx_args
GetTransactionInfoFromData(const std::vector<uint8_t>& data);

}  // namespace hns_wallet

#endif  // HNS_COMPONENTS_HNS_WALLET_BROWSER_ETH_DATA_PARSER_H_
