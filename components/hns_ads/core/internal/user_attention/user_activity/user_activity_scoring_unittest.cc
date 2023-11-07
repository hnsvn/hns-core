/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/user_attention/user_activity/user_activity_scoring.h"

#include "hns/components/hns_ads/core/internal/common/unittest/unittest_base.h"
#include "hns/components/hns_ads/core/internal/user_attention/user_activity/user_activity_manager.h"
#include "hns/components/hns_ads/core/internal/user_attention/user_activity/user_activity_trigger_info.h"
#include "hns/components/hns_ads/core/internal/user_attention/user_activity/user_activity_util.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads {

class HnsAdsUserActivityScoringTest : public UnitTestBase {};

TEST_F(HnsAdsUserActivityScoringTest, GetUserActivityScore) {
  // Arrange
  const UserActivityTriggerList triggers =
      ToUserActivityTriggers("06=.3;0D1406=1.0;0D14=0.5");

  UserActivityManager::GetInstance().RecordEvent(
      UserActivityEventType::kClickedLink);
  UserActivityManager::GetInstance().RecordEvent(
      UserActivityEventType::kClickedReloadButton);
  UserActivityManager::GetInstance().RecordEvent(
      UserActivityEventType::kOpenedNewTab);
  UserActivityManager::GetInstance().RecordEvent(
      UserActivityEventType::kTypedUrl);
  UserActivityManager::GetInstance().RecordEvent(
      UserActivityEventType::kTabStartedPlayingMedia);
  UserActivityManager::GetInstance().RecordEvent(
      UserActivityEventType::kOpenedNewTab);
  UserActivityManager::GetInstance().RecordEvent(
      UserActivityEventType::kTypedUrl);
  UserActivityManager::GetInstance().RecordEvent(
      UserActivityEventType::kClickedLink);

  const UserActivityEventList events =
      UserActivityManager::GetInstance().GetHistoryForTimeWindow(
          base::Hours(1));

  // Act
  const double score = GetUserActivityScore(triggers, events);

  // Assert
  EXPECT_EQ(1.8, score);
}

TEST_F(HnsAdsUserActivityScoringTest, GetUserActivityScoreForTimeWindow) {
  // Arrange
  const UserActivityTriggerList triggers =
      ToUserActivityTriggers("06=.3;0D1406=1.0;0D14=0.5");

  UserActivityManager::GetInstance().RecordEvent(
      UserActivityEventType::kClickedLink);
  AdvanceClockBy(base::Hours(2));
  UserActivityManager::GetInstance().RecordEvent(
      UserActivityEventType::kClickedReloadButton);
  UserActivityManager::GetInstance().RecordEvent(
      UserActivityEventType::kOpenedNewTab);
  UserActivityManager::GetInstance().RecordEvent(
      UserActivityEventType::kTypedUrl);
  UserActivityManager::GetInstance().RecordEvent(
      UserActivityEventType::kTabStartedPlayingMedia);
  UserActivityManager::GetInstance().RecordEvent(
      UserActivityEventType::kOpenedNewTab);
  UserActivityManager::GetInstance().RecordEvent(
      UserActivityEventType::kTypedUrl);
  UserActivityManager::GetInstance().RecordEvent(
      UserActivityEventType::kClickedLink);

  const UserActivityEventList events =
      UserActivityManager::GetInstance().GetHistoryForTimeWindow(
          base::Hours(1));

  // Act
  const double score = GetUserActivityScore(triggers, events);

  // Assert
  EXPECT_EQ(1.5, score);
}

TEST_F(HnsAdsUserActivityScoringTest,
       GetUserActivityScoreForInvalidEventSequence) {
  // Arrange
  const UserActivityTriggerList triggers = ToUserActivityTriggers("INVALID");

  UserActivityManager::GetInstance().RecordEvent(
      UserActivityEventType::kClickedLink);
  UserActivityManager::GetInstance().RecordEvent(
      UserActivityEventType::kClickedReloadButton);
  UserActivityManager::GetInstance().RecordEvent(
      UserActivityEventType::kOpenedNewTab);
  UserActivityManager::GetInstance().RecordEvent(
      UserActivityEventType::kTypedUrl);
  UserActivityManager::GetInstance().RecordEvent(
      UserActivityEventType::kTabStartedPlayingMedia);
  UserActivityManager::GetInstance().RecordEvent(
      UserActivityEventType::kOpenedNewTab);
  UserActivityManager::GetInstance().RecordEvent(
      UserActivityEventType::kTypedUrl);
  UserActivityManager::GetInstance().RecordEvent(
      UserActivityEventType::kClickedLink);

  const UserActivityEventList events =
      UserActivityManager::GetInstance().GetHistoryForTimeWindow(
          base::Hours(1));

  // Act
  const double score = GetUserActivityScore(triggers, events);

  // Assert
  EXPECT_EQ(0.0, score);
}

TEST_F(HnsAdsUserActivityScoringTest,
       GetUserActivityScoreForMalformedEventSequence) {
  // Arrange
  const UserActivityTriggerList triggers =
      ToUserActivityTriggers("06=1;0D1406=1.0;=0.5");

  UserActivityManager::GetInstance().RecordEvent(
      UserActivityEventType::kClickedLink);
  UserActivityManager::GetInstance().RecordEvent(
      UserActivityEventType::kClickedReloadButton);
  UserActivityManager::GetInstance().RecordEvent(
      UserActivityEventType::kOpenedNewTab);
  UserActivityManager::GetInstance().RecordEvent(
      UserActivityEventType::kTypedUrl);
  UserActivityManager::GetInstance().RecordEvent(
      UserActivityEventType::kTabStartedPlayingMedia);
  UserActivityManager::GetInstance().RecordEvent(
      UserActivityEventType::kOpenedNewTab);
  UserActivityManager::GetInstance().RecordEvent(
      UserActivityEventType::kTypedUrl);
  UserActivityManager::GetInstance().RecordEvent(
      UserActivityEventType::kClickedLink);

  const UserActivityEventList events =
      UserActivityManager::GetInstance().GetHistoryForTimeWindow(
          base::Hours(1));

  // Act
  const double score = GetUserActivityScore(triggers, events);

  // Assert
  EXPECT_EQ(2.0, score);
}

TEST_F(HnsAdsUserActivityScoringTest,
       GetUserActivityScoreForMixedCaseEventSequence) {
  // Arrange
  const UserActivityTriggerList triggers =
      ToUserActivityTriggers("06=.3;0d1406=1.0;0D14=0.5");

  UserActivityManager::GetInstance().RecordEvent(
      UserActivityEventType::kClickedLink);
  UserActivityManager::GetInstance().RecordEvent(
      UserActivityEventType::kClickedReloadButton);
  UserActivityManager::GetInstance().RecordEvent(
      UserActivityEventType::kOpenedNewTab);
  UserActivityManager::GetInstance().RecordEvent(
      UserActivityEventType::kTypedUrl);
  UserActivityManager::GetInstance().RecordEvent(
      UserActivityEventType::kTabStartedPlayingMedia);
  UserActivityManager::GetInstance().RecordEvent(
      UserActivityEventType::kOpenedNewTab);
  UserActivityManager::GetInstance().RecordEvent(
      UserActivityEventType::kTypedUrl);
  UserActivityManager::GetInstance().RecordEvent(
      UserActivityEventType::kClickedLink);

  const UserActivityEventList events =
      UserActivityManager::GetInstance().GetHistoryForTimeWindow(
          base::Hours(1));

  // Act
  const double score = GetUserActivityScore(triggers, events);

  // Assert
  EXPECT_EQ(1.8, score);
}

TEST_F(HnsAdsUserActivityScoringTest,
       GetUserActivityScoreForEmptyEventSequence) {
  // Arrange
  const UserActivityTriggerList triggers =
      ToUserActivityTriggers(/*param_value*/ {});

  UserActivityManager::GetInstance().RecordEvent(
      UserActivityEventType::kClickedLink);
  UserActivityManager::GetInstance().RecordEvent(
      UserActivityEventType::kClickedReloadButton);
  UserActivityManager::GetInstance().RecordEvent(
      UserActivityEventType::kOpenedNewTab);
  UserActivityManager::GetInstance().RecordEvent(
      UserActivityEventType::kTypedUrl);
  UserActivityManager::GetInstance().RecordEvent(
      UserActivityEventType::kTabStartedPlayingMedia);
  UserActivityManager::GetInstance().RecordEvent(
      UserActivityEventType::kOpenedNewTab);
  UserActivityManager::GetInstance().RecordEvent(
      UserActivityEventType::kTypedUrl);
  UserActivityManager::GetInstance().RecordEvent(
      UserActivityEventType::kClickedLink);

  const UserActivityEventList events =
      UserActivityManager::GetInstance().GetHistoryForTimeWindow(
          base::Hours(1));

  // Act
  const double score = GetUserActivityScore(triggers, events);

  // Assert
  EXPECT_EQ(0.0, score);
}

}  // namespace hns_ads
