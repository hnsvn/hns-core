/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_WALLET_BROWSER_SWAP_REQUEST_HELPER_H_
#define HNS_COMPONENTS_HNS_WALLET_BROWSER_SWAP_REQUEST_HELPER_H_

#include <string>
#include "hns/components/hns_wallet/common/hns_wallet.mojom.h"

namespace hns_wallet {

absl::optional<std::string> EncodeJupiterTransactionParams(
    mojom::JupiterSwapParamsPtr params,
    bool has_fee);

}  // namespace hns_wallet

#endif  // HNS_COMPONENTS_HNS_WALLET_BROWSER_SWAP_REQUEST_HELPER_H_
