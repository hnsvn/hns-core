/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_REWARDS_CORE_ENDPOINTS_RESULT_FOR_H_
#define HNS_COMPONENTS_HNS_REWARDS_CORE_ENDPOINTS_RESULT_FOR_H_

namespace hns_rewards::internal::endpoints {

template <typename>
inline constexpr bool dependent_false_v = false;

template <typename Endpoint>
struct ResultFor {
  static_assert(dependent_false_v<Endpoint>,
                "Please explicitly specialize ResultFor<> for your endpoint!");
};

}  // namespace hns_rewards::internal::endpoints

#endif  // HNS_COMPONENTS_HNS_REWARDS_CORE_ENDPOINTS_RESULT_FOR_H_
