/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_REWARDS_CORE_SKU_SKU_UTIL_H_
#define HNS_COMPONENTS_HNS_REWARDS_CORE_SKU_SKU_UTIL_H_

#include <string>
#include <vector>

#include "hns/components/hns_rewards/core/mojom_structs.h"

namespace hns_rewards::internal {
namespace sku {

std::string GetHnsDestination(const std::string& wallet_type);

std::string GetUpholdDestination();

std::string GetGeminiDestination();

}  // namespace sku
}  // namespace hns_rewards::internal

#endif  // HNS_COMPONENTS_HNS_REWARDS_CORE_SKU_SKU_UTIL_H_
