/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_TARGETING_BEHAVIORAL_MULTI_ARMED_BANDITS_MODEL_EPSILON_GREEDY_BANDIT_MODEL_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_TARGETING_BEHAVIORAL_MULTI_ARMED_BANDITS_MODEL_EPSILON_GREEDY_BANDIT_MODEL_H_

#include "hns/components/hns_ads/core/internal/segments/segment_alias.h"

namespace hns_ads {

class EpsilonGreedyBanditModel final {
 public:
  SegmentList GetSegments() const;
};

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_TARGETING_BEHAVIORAL_MULTI_ARMED_BANDITS_MODEL_EPSILON_GREEDY_BANDIT_MODEL_H_
