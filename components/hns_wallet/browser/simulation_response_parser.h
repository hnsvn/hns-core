/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_WALLET_BROWSER_SIMULATION_RESPONSE_PARSER_H_
#define HNS_COMPONENTS_HNS_WALLET_BROWSER_SIMULATION_RESPONSE_PARSER_H_

#include <string>

#include "hns/components/hns_wallet/common/hns_wallet.mojom.h"

namespace hns_wallet {

namespace evm {

mojom::EVMSimulationResponsePtr ParseSimulationResponse(
    const base::Value& json_value);

}  // namespace evm

namespace solana {

mojom::SolanaSimulationResponsePtr ParseSimulationResponse(
    const base::Value& json_value);

}  // namespace solana

absl::optional<std::string> ParseSimulationErrorResponse(
    const base::Value& json_value);

}  // namespace hns_wallet

#endif  // HNS_COMPONENTS_HNS_WALLET_BROWSER_SIMULATION_RESPONSE_PARSER_H_
