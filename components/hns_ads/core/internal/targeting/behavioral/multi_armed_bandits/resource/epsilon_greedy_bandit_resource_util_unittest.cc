/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/targeting/behavioral/multi_armed_bandits/resource/epsilon_greedy_bandit_resource_util.h"

#include "hns/components/hns_ads/core/internal/common/unittest/unittest_base.h"
#include "hns/components/hns_ads/core/internal/segments/segment_alias.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads {

class HnsAdsEpsilonGreedyBanditResourceUtilTest : public UnitTestBase {};

TEST_F(HnsAdsEpsilonGreedyBanditResourceUtilTest, SetEligibleSegments) {
  const SegmentList expected_segments = {"foo", "bar"};

  // Arrange

  // Act
  SetEpsilonGreedyBanditEligibleSegments(expected_segments);

  // Assert
  EXPECT_EQ(expected_segments, GetEpsilonGreedyBanditEligibleSegments());
}

TEST_F(HnsAdsEpsilonGreedyBanditResourceUtilTest, SetNoEligibleSegments) {
  // Arrange

  // Act
  SetEpsilonGreedyBanditEligibleSegments(/*segments*/ {});

  // Assert
  const SegmentList segments = GetEpsilonGreedyBanditEligibleSegments();
  EXPECT_TRUE(segments.empty());
}

}  // namespace hns_ads
