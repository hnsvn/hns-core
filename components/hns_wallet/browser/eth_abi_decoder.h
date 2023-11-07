/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_WALLET_BROWSER_ETH_ABI_DECODER_H_
#define HNS_COMPONENTS_HNS_WALLET_BROWSER_ETH_ABI_DECODER_H_

#include <string>
#include <tuple>
#include <vector>
#include "base/values.h"

namespace hns_wallet {

absl::optional<std::vector<std::string>> UniswapEncodedPathDecode(
    const std::string& encoded_path);

absl::optional<std::tuple<std::vector<std::string>,   // tx_params
                          std::vector<std::string>>>  // tx_args
ABIDecode(const std::vector<std::string>& types,
          const std::vector<uint8_t>& data);

}  // namespace hns_wallet

#endif  // HNS_COMPONENTS_HNS_WALLET_BROWSER_ETH_ABI_DECODER_H_
