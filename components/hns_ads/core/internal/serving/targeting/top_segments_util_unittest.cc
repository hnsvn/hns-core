/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/serving/targeting/top_segments_util.h"

#include "hns/components/hns_ads/core/internal/common/unittest/unittest_base.h"
#include "hns/components/hns_ads/core/internal/serving/targeting/user_model_builder_unittest_util.h"
#include "hns/components/hns_ads/core/internal/serving/targeting/user_model_info.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads {

namespace {
constexpr int kSegmentsMaxCount = 3;
}  // namespace

class HnsAdsTopSegmentsUtilTest : public UnitTestBase {};

TEST_F(HnsAdsTopSegmentsUtilTest, GetTopChildSegments) {
  // Arrange
  const UserModelInfo user_model = BuildUserModelForTesting(
      /*intent_segments*/ {"intent-1", "intent-2"},
      /*latent_interest_segments*/ {"latent_interest-1", "latent_interest-2"},
      /*interest_segments*/ {"interest-1", "interest-2"},
      /*text_embedding_html_events*/ {});

  // Act
  const SegmentList segments =
      GetTopSegments(user_model, kSegmentsMaxCount, /*parent_only*/ false);

  // Assert
  const SegmentList expected_segments = {
      "intent-1",          "intent-2",   "latent_interest-1",
      "latent_interest-2", "interest-1", "interest-2"};

  EXPECT_EQ(expected_segments, segments);
}

TEST_F(HnsAdsTopSegmentsUtilTest, GetTopChildSegmentsForEmptyUserModel) {
  // Arrange
  const UserModelInfo user_model;

  // Act
  const SegmentList segments =
      GetTopSegments(user_model, kSegmentsMaxCount, /*parent_only*/ false);

  // Assert
  EXPECT_TRUE(segments.empty());
}

TEST_F(HnsAdsTopSegmentsUtilTest, GetTopParentSegments) {
  // Arrange
  const UserModelInfo user_model = BuildUserModelForTesting(
      /*intent_segments*/ {"intent-1", "intent-2"},
      /*latent_interest_segments*/ {"latent_interest-1", "latent_interest-2"},
      /*interest_segments*/ {"interest-1", "interest-2"},
      /*text_embedding_html_events*/ {});

  // Act
  const SegmentList segments =
      GetTopSegments(user_model, kSegmentsMaxCount, /*parent_only*/ true);

  // Assert
  const SegmentList expected_segments = {"intent", "latent_interest",
                                         "interest"};

  EXPECT_EQ(expected_segments, segments);
}

TEST_F(HnsAdsTopSegmentsUtilTest, GetTopParentSegmentsForEmptyUserModel) {
  // Arrange
  const UserModelInfo user_model;

  // Act
  const SegmentList segments =
      GetTopSegments(user_model, kSegmentsMaxCount, /*parent_only*/ true);

  // Assert
  EXPECT_TRUE(segments.empty());
}

}  // namespace hns_ads
