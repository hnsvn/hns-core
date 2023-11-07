/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_WALLET_BROWSER_RLP_ENCODE_H_
#define HNS_COMPONENTS_HNS_WALLET_BROWSER_RLP_ENCODE_H_

#include <string>

#include "base/values.h"
#include "hns/components/hns_wallet/common/hns_wallet_types.h"

namespace hns_wallet {

// Converts a uint256_t value into a blob value type
base::Value::BlobStorage RLPUint256ToBlob(uint256_t input);

// Recursive Length Prefix (RLP) encoding of base::Values consisting of string,
// blob, or int data
std::string RLPEncode(base::Value val);

}  // namespace hns_wallet

#endif  // HNS_COMPONENTS_HNS_WALLET_BROWSER_RLP_ENCODE_H_
