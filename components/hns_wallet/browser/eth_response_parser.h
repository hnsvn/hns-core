/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_WALLET_BROWSER_ETH_RESPONSE_PARSER_H_
#define HNS_COMPONENTS_HNS_WALLET_BROWSER_ETH_RESPONSE_PARSER_H_

#include <string>
#include <vector>

#include "base/values.h"
#include "hns/components/hns_wallet/common/hns_wallet.mojom.h"
#include "hns/components/hns_wallet/common/hns_wallet_types.h"

// TODO(apaymyshev): refactor utility methods to return absl::optional instead
// of bool + out-parameter.

namespace hns_wallet {

namespace eth {

bool ParseStringResult(const base::Value& json_value, std::string* value);
bool ParseAddressResult(const base::Value& json_value, std::string* address);
bool ParseEthGetBlockNumber(const base::Value& json_value,
                            uint256_t* block_num);
bool ParseEthGetFeeHistory(const base::Value& json_value,
                           std::vector<std::string>* base_fee_per_gas,
                           std::vector<double>* gas_used_ratio,
                           std::string* oldest_block,
                           std::vector<std::vector<std::string>>* reward);
// Returns the balance of the account of given address.
bool ParseEthGetBalance(const base::Value& json_value,
                        std::string* hex_balance);
bool ParseEthGetTransactionCount(const base::Value& json_value,
                                 uint256_t* count);
bool ParseEthGetTransactionReceipt(const base::Value& json_value,
                                   TransactionReceipt* receipt);
absl::optional<std::string> ParseEthSendRawTransaction(
    const base::Value& json_value);
absl::optional<std::string> ParseEthCall(const base::Value& json_value);
absl::optional<std::vector<std::string>> DecodeEthCallResponse(
    const std::string& data,
    const std::vector<std::string>& abi_types);
absl::optional<std::vector<absl::optional<std::string>>>
DecodeGetERC20TokenBalancesEthCallResponse(const std::string& data);
absl::optional<std::string> ParseEthEstimateGas(const base::Value& json_value);
absl::optional<std::string> ParseEthGasPrice(const base::Value& json_value);
bool ParseEthGetLogs(const base::Value& json_value, std::vector<Log>* logs);

bool ParseEnsResolverContentHash(const base::Value& json_value,
                                 std::vector<uint8_t>* content_hash);
absl::optional<std::vector<std::string>>
ParseUnstoppableDomainsProxyReaderGetMany(const base::Value& json_value);

absl::optional<std::string> ParseUnstoppableDomainsProxyReaderGet(
    const base::Value& json_value);

// Get the JSON included in a data URI with a mime type application/json
bool ParseDataURIAndExtractJSON(const GURL url, std::string* json);

bool ParseTokenUri(const base::Value& json_value, GURL* url);

}  // namespace eth

}  // namespace hns_wallet

#endif  // HNS_COMPONENTS_HNS_WALLET_BROWSER_ETH_RESPONSE_PARSER_H_
