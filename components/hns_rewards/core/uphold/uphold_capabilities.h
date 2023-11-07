/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_REWARDS_CORE_UPHOLD_UPHOLD_CAPABILITIES_H_
#define HNS_COMPONENTS_HNS_REWARDS_CORE_UPHOLD_UPHOLD_CAPABILITIES_H_

#include "third_party/abseil-cpp/absl/types/optional.h"

namespace hns_rewards::internal {
namespace uphold {

struct Capabilities {
  absl::optional<bool> can_receive;
  absl::optional<bool> can_send;
};

}  // namespace uphold
}  // namespace hns_rewards::internal

#endif  // HNS_COMPONENTS_HNS_REWARDS_CORE_UPHOLD_UPHOLD_CAPABILITIES_H_
