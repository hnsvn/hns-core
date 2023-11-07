/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_REWARDS_CORE_UPHOLD_UPHOLD_USER_H_
#define HNS_COMPONENTS_HNS_REWARDS_CORE_UPHOLD_UPHOLD_USER_H_

#include <string>

namespace hns_rewards::internal::uphold {

struct User {
  User();

  std::string name = "";
  std::string member_id = "";
  std::string country_id = "";
  bool bat_not_allowed = true;
};

}  // namespace hns_rewards::internal::uphold

#endif  // HNS_COMPONENTS_HNS_REWARDS_CORE_UPHOLD_UPHOLD_USER_H_
