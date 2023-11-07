/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_WALLET_BROWSER_ETH_TOPICS_BUILDER_H_
#define HNS_COMPONENTS_HNS_WALLET_BROWSER_ETH_TOPICS_BUILDER_H_

#include <string>
#include <vector>
#include "base/values.h"

namespace hns_wallet {

// Returns topics that match all ERC20 transfer event logs for the given
// to_account_addresses
bool MakeAssetDiscoveryTopics(
    const std::vector<std::string>& to_account_addresses,
    base::Value::List* topics);

}  // namespace hns_wallet

#endif  // HNS_COMPONENTS_HNS_WALLET_BROWSER_ETH_TOPICS_BUILDER_H_
