/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_TARGETING_BEHAVIORAL_MULTI_ARMED_BANDITS_EPSILON_GREEDY_BANDIT_ARM_INFO_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_TARGETING_BEHAVIORAL_MULTI_ARMED_BANDITS_EPSILON_GREEDY_BANDIT_ARM_INFO_H_

#include <string>
#include <vector>

namespace hns_ads {

struct EpsilonGreedyBanditArmInfo final {
  bool operator==(const EpsilonGreedyBanditArmInfo&) const;
  bool operator!=(const EpsilonGreedyBanditArmInfo&) const;

  [[nodiscard]] bool IsValid() const;

  std::string segment;
  double value = 0.0;
  int pulls = 0;
};

using EpsilonGreedyBanditArmList = std::vector<EpsilonGreedyBanditArmInfo>;

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_TARGETING_BEHAVIORAL_MULTI_ARMED_BANDITS_EPSILON_GREEDY_BANDIT_ARM_INFO_H_
