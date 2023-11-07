/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_WALLET_COMMON_STRING_UTILS_H_
#define HNS_COMPONENTS_HNS_WALLET_COMMON_STRING_UTILS_H_

#include <string>
#include <vector>

#include "hns/components/hns_wallet/common/hns_wallet_types.h"

namespace hns_wallet {

// Returns true if a string contains only 0-9 digits
bool IsValidBase10String(const std::string& input);

// Takes a base-10 string as input and converts it to a uint256_t
bool Base10ValueToUint256(const std::string& input, uint256_t* out);
bool Base10ValueToInt256(const std::string& input, int256_t* out);

}  // namespace hns_wallet

#endif  // HNS_COMPONENTS_HNS_WALLET_COMMON_STRING_UTILS_H_
