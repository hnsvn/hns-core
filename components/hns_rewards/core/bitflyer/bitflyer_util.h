/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_REWARDS_CORE_BITFLYER_BITFLYER_UTIL_H_
#define HNS_COMPONENTS_HNS_REWARDS_CORE_BITFLYER_BITFLYER_UTIL_H_

#include <string>
#include <vector>

#include "hns/components/hns_rewards/common/mojom/rewards_types.mojom.h"

namespace hns_rewards::internal::bitflyer {

std::string GetClientId();

std::string GetClientSecret();

std::string GetFeeAddress();

mojom::ExternalWalletPtr GenerateLinks(mojom::ExternalWalletPtr wallet);

}  // namespace hns_rewards::internal::bitflyer

namespace hns_rewards::internal::endpoint::bitflyer {

std::vector<std::string> RequestAuthorization(const std::string& token = "");

std::string GetServerUrl(const std::string& path);

}  // namespace hns_rewards::internal::endpoint::bitflyer

#endif  // HNS_COMPONENTS_HNS_REWARDS_CORE_BITFLYER_BITFLYER_UTIL_H_
