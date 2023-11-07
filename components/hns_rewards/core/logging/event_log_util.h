/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_REWARDS_CORE_LOGGING_EVENT_LOG_UTIL_H_
#define HNS_COMPONENTS_HNS_REWARDS_CORE_LOGGING_EVENT_LOG_UTIL_H_

#include <string>

#include "hns/components/hns_rewards/common/mojom/rewards_types.mojom.h"
#include "hns/components/hns_rewards/core/mojom_structs.h"

namespace hns_rewards::internal {
namespace log {
std::string GetEventLogKeyForLinkingResult(mojom::ConnectExternalWalletError);
}  // namespace log
}  // namespace hns_rewards::internal

#endif  // HNS_COMPONENTS_HNS_REWARDS_CORE_LOGGING_EVENT_LOG_UTIL_H_
