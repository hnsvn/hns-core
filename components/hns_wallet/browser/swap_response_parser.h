/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_WALLET_BROWSER_SWAP_RESPONSE_PARSER_H_
#define HNS_COMPONENTS_HNS_WALLET_BROWSER_SWAP_RESPONSE_PARSER_H_

#include <string>

#include "hns/components/hns_wallet/common/hns_wallet.mojom.h"

namespace hns_wallet {

mojom::SwapResponsePtr ParseSwapResponse(const base::Value& json_value,
                                         bool expect_transaction_data);
mojom::SwapErrorResponsePtr ParseSwapErrorResponse(
    const base::Value& json_value);

mojom::JupiterQuotePtr ParseJupiterQuote(const base::Value& json_value);
mojom::JupiterSwapTransactionsPtr ParseJupiterSwapTransactions(
    const base::Value& json_value);
mojom::JupiterErrorResponsePtr ParseJupiterErrorResponse(
    const base::Value& json_value);
absl::optional<std::string> ConvertAllNumbersToString(const std::string& json);
}  // namespace hns_wallet

#endif  // HNS_COMPONENTS_HNS_WALLET_BROWSER_SWAP_RESPONSE_PARSER_H_
