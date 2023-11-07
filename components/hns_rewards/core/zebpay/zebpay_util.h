/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_REWARDS_CORE_ZEBPAY_ZEBPAY_UTIL_H_
#define HNS_COMPONENTS_HNS_REWARDS_CORE_ZEBPAY_ZEBPAY_UTIL_H_

#include <string>
#include <vector>

#include "hns/components/hns_rewards/common/mojom/rewards_types.mojom.h"

namespace hns_rewards::internal::zebpay {

std::string GetClientId();

std::string GetClientSecret();

mojom::ExternalWalletPtr GenerateLinks(mojom::ExternalWalletPtr wallet);

}  // namespace hns_rewards::internal::zebpay

namespace hns_rewards::internal::endpoint::zebpay {

std::vector<std::string> RequestAuthorization(const std::string& token = "");

std::string GetApiServerUrl(const std::string& path);
std::string GetOauthServerUrl(const std::string& path);

}  // namespace hns_rewards::internal::endpoint::zebpay

#endif  // HNS_COMPONENTS_HNS_REWARDS_CORE_ZEBPAY_ZEBPAY_UTIL_H_
