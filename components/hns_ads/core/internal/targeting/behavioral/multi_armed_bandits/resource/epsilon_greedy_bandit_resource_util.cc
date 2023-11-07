/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/targeting/behavioral/multi_armed_bandits/resource/epsilon_greedy_bandit_resource_util.h"

#include "base/values.h"
#include "hns/components/hns_ads/core/internal/client/ads_client_helper.h"
#include "hns/components/hns_ads/core/internal/segments/segment_value_util.h"
#include "hns/components/hns_ads/core/public/prefs/pref_names.h"
#include "third_party/abseil-cpp/absl/types/optional.h"

namespace hns_ads {

void ResetEpsilonGreedyBanditEligibleSegments() {
  AdsClientHelper::GetInstance()->ClearPref(
      prefs::kEpsilonGreedyBanditEligibleSegments);
}

void SetEpsilonGreedyBanditEligibleSegments(const SegmentList& segments) {
  AdsClientHelper::GetInstance()->SetListPref(
      prefs::kEpsilonGreedyBanditEligibleSegments, SegmentsToValue(segments));
}

SegmentList GetEpsilonGreedyBanditEligibleSegments() {
  const absl::optional<base::Value::List> list =
      AdsClientHelper::GetInstance()->GetListPref(
          prefs::kEpsilonGreedyBanditEligibleSegments);
  if (!list) {
    return {};
  }

  return SegmentsFromValue(*list);
}

}  // namespace hns_ads
