/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_REWARDS_CORE_ENDPOINT_API_API_UTIL_H_
#define HNS_COMPONENTS_HNS_REWARDS_CORE_ENDPOINT_API_API_UTIL_H_

#include <string>

namespace hns_rewards::internal {
namespace endpoint {
namespace api {

std::string GetServerUrl(const std::string& path);

}  // namespace api
}  // namespace endpoint
}  // namespace hns_rewards::internal

#endif  // HNS_COMPONENTS_HNS_REWARDS_CORE_ENDPOINT_API_API_UTIL_H_
