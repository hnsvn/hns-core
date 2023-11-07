/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_WALLET_BROWSER_ASSET_RATIO_RESPONSE_PARSER_H_
#define HNS_COMPONENTS_HNS_WALLET_BROWSER_ASSET_RATIO_RESPONSE_PARSER_H_

#include <string>
#include <utility>
#include <vector>

#include "base/values.h"
#include "hns/components/hns_wallet/common/hns_wallet.mojom.h"
#include "third_party/abseil-cpp/absl/types/optional.h"

namespace hns_wallet {

absl::optional<std::string> ParseSardineAuthToken(
    const base::Value& json_value);

bool ParseAssetPrice(const base::Value& json_value,
                     const std::vector<std::string>& from_assets,
                     const std::vector<std::string>& to_assets,
                     std::vector<mojom::AssetPricePtr>* values);
bool ParseAssetPriceHistory(const base::Value& json_value,
                            std::vector<mojom::AssetTimePricePtr>* values);
absl::optional<std::vector<mojom::CoinMarketPtr>> ParseCoinMarkets(
    const base::Value& json_value);

absl::optional<std::string> ParseStripeBuyURL(const base::Value& json_value);

mojom::BlockchainTokenPtr ParseTokenInfo(const base::Value& json_value,
                                         const std::string& chain_id,
                                         mojom::CoinType coin);

}  // namespace hns_wallet

#endif  // HNS_COMPONENTS_HNS_WALLET_BROWSER_ASSET_RATIO_RESPONSE_PARSER_H_