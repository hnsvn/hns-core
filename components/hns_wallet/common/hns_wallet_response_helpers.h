/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_WALLET_COMMON_HNS_WALLET_RESPONSE_HELPERS_H_
#define HNS_COMPONENTS_HNS_WALLET_COMMON_HNS_WALLET_RESPONSE_HELPERS_H_

#include <memory>
#include <string>

#include "base/values.h"
#include "hns/components/hns_wallet/common/hns_wallet.mojom.h"

namespace hns_wallet {

base::Value GetProviderRequestReturnFromEthJsonResponse(
    int http_code,
    const base::Value& json_value,
    bool* reject);
base::Value GetProviderErrorDictionary(mojom::ProviderError code,
                                       const std::string& message);
base::Value GetSolanaProviderErrorDictionary(mojom::SolanaProviderError code,
                                             const std::string& message);
base::Value ToProviderResponse(base::Value id,
                               base::Value* result,
                               base::Value* error);

}  // namespace hns_wallet

#endif  // HNS_COMPONENTS_HNS_WALLET_COMMON_HNS_WALLET_RESPONSE_HELPERS_H_
