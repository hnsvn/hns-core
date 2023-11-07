/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_REWARDS_CORE_ENDPOINTS_HNS_GET_PARAMETERS_UTILS_H_
#define HNS_COMPONENTS_HNS_REWARDS_CORE_ENDPOINTS_HNS_GET_PARAMETERS_UTILS_H_

#include <string>

#include "base/containers/flat_map.h"
#include "base/values.h"
#include "hns/components/hns_rewards/common/mojom/rewards.mojom.h"
#include "third_party/abseil-cpp/absl/types/optional.h"

namespace hns_rewards::internal::endpoints {

absl::optional<base::flat_map<std::string, mojom::RegionsPtr>>
GetWalletProviderRegions(const base::Value::Dict&);

}  // namespace hns_rewards::internal::endpoints

#endif  // HNS_COMPONENTS_HNS_REWARDS_CORE_ENDPOINTS_HNS_GET_PARAMETERS_UTILS_H_
