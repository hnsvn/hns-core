/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/user_attention/user_activity/user_activity_manager.h"

#include "base/ranges/algorithm.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_base.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_time_util.h"
#include "hns/components/hns_ads/core/internal/settings/settings_unittest_util.h"
#include "hns/components/hns_ads/core/internal/user_attention/user_activity/user_activity_constants.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads {

class HnsAdsUserActivityManagerTest : public UnitTestBase {};

TEST_F(HnsAdsUserActivityManagerTest, RecordInitializedAdsEvent) {
  // Arrange
  const UserActivityEventType event_type =
      UserActivityEventType::kInitializedAds;

  // Act
  UserActivityManager::GetInstance().RecordEvent(event_type);

  // Assert
  const UserActivityEventList events =
      UserActivityManager::GetInstance().GetHistoryForTimeWindow(
          base::Hours(1));

  UserActivityEventList expected_events;
  UserActivityEventInfo event;
  event.type = event_type;
  event.created_at = Now();
  expected_events.push_back(event);

  EXPECT_TRUE(base::ranges::equal(expected_events, events));
}

TEST_F(HnsAdsUserActivityManagerTest, RecordBrowserDidEnterForegroundEvent) {
  // Arrange
  const UserActivityEventType event_type =
      UserActivityEventType::kBrowserDidEnterForeground;

  // Act
  UserActivityManager::GetInstance().RecordEvent(event_type);

  // Assert
  const UserActivityEventList events =
      UserActivityManager::GetInstance().GetHistoryForTimeWindow(
          base::Hours(1));

  UserActivityEventList expected_events;
  UserActivityEventInfo event;
  event.type = event_type;
  event.created_at = Now();
  expected_events.push_back(event);

  EXPECT_TRUE(base::ranges::equal(expected_events, events));
}

TEST_F(HnsAdsUserActivityManagerTest, RecordBrowserDidEnterBackgroundEvent) {
  // Arrange
  const UserActivityEventType event_type =
      UserActivityEventType::kBrowserDidEnterBackground;

  // Act
  UserActivityManager::GetInstance().RecordEvent(event_type);

  // Assert
  const UserActivityEventList events =
      UserActivityManager::GetInstance().GetHistoryForTimeWindow(
          base::Hours(1));

  UserActivityEventList expected_events;
  UserActivityEventInfo event;
  event.type = event_type;
  event.created_at = Now();
  expected_events.push_back(event);

  EXPECT_TRUE(base::ranges::equal(expected_events, events));
}

TEST_F(HnsAdsUserActivityManagerTest,
       RecordClickedBackOrForwardNavigationButtonsEvent) {
  // Arrange
  const UserActivityEventType event_type =
      UserActivityEventType::kClickedBackOrForwardNavigationButtons;

  // Act
  UserActivityManager::GetInstance().RecordEvent(event_type);

  // Assert
  const UserActivityEventList events =
      UserActivityManager::GetInstance().GetHistoryForTimeWindow(
          base::Hours(1));

  UserActivityEventList expected_events;
  UserActivityEventInfo event;
  event.type = event_type;
  event.created_at = Now();
  expected_events.push_back(event);

  EXPECT_TRUE(base::ranges::equal(expected_events, events));
}

TEST_F(HnsAdsUserActivityManagerTest, RecordClickedBookmarkEvent) {
  // Arrange
  const UserActivityEventType event_type =
      UserActivityEventType::kClickedBookmark;

  // Act
  UserActivityManager::GetInstance().RecordEvent(event_type);

  // Assert
  const UserActivityEventList events =
      UserActivityManager::GetInstance().GetHistoryForTimeWindow(
          base::Hours(1));

  UserActivityEventList expected_events;
  UserActivityEventInfo event;
  event.type = event_type;
  event.created_at = Now();
  expected_events.push_back(event);

  EXPECT_TRUE(base::ranges::equal(expected_events, events));
}

TEST_F(HnsAdsUserActivityManagerTest, RecordClickedHomePageButtonEvent) {
  // Arrange
  const UserActivityEventType event_type =
      UserActivityEventType::kClickedHomePageButton;

  // Act
  UserActivityManager::GetInstance().RecordEvent(event_type);

  // Assert
  const UserActivityEventList events =
      UserActivityManager::GetInstance().GetHistoryForTimeWindow(
          base::Hours(1));

  UserActivityEventList expected_events;
  UserActivityEventInfo event;
  event.type = event_type;
  event.created_at = Now();
  expected_events.push_back(event);

  EXPECT_TRUE(base::ranges::equal(expected_events, events));
}

TEST_F(HnsAdsUserActivityManagerTest, RecordClickedLinkEvent) {
  // Arrange
  const UserActivityEventType event_type = UserActivityEventType::kClickedLink;

  // Act
  UserActivityManager::GetInstance().RecordEvent(event_type);

  // Assert
  const UserActivityEventList events =
      UserActivityManager::GetInstance().GetHistoryForTimeWindow(
          base::Hours(1));

  UserActivityEventList expected_events;
  UserActivityEventInfo event;
  event.type = event_type;
  event.created_at = Now();
  expected_events.push_back(event);

  EXPECT_TRUE(base::ranges::equal(expected_events, events));
}

TEST_F(HnsAdsUserActivityManagerTest, RecordClickedReloadButtonEvent) {
  // Arrange
  const UserActivityEventType event_type =
      UserActivityEventType::kClickedReloadButton;

  // Act
  UserActivityManager::GetInstance().RecordEvent(event_type);

  // Assert
  const UserActivityEventList events =
      UserActivityManager::GetInstance().GetHistoryForTimeWindow(
          base::Hours(1));

  UserActivityEventList expected_events;
  UserActivityEventInfo event;
  event.type = event_type;
  event.created_at = Now();
  expected_events.push_back(event);

  EXPECT_TRUE(base::ranges::equal(expected_events, events));
}

TEST_F(HnsAdsUserActivityManagerTest, RecordClosedTabEvent) {
  // Arrange
  const UserActivityEventType event_type = UserActivityEventType::kClosedTab;

  // Act
  UserActivityManager::GetInstance().RecordEvent(event_type);

  // Assert
  const UserActivityEventList events =
      UserActivityManager::GetInstance().GetHistoryForTimeWindow(
          base::Hours(1));

  UserActivityEventList expected_events;
  UserActivityEventInfo event;
  event.type = event_type;
  event.created_at = Now();
  expected_events.push_back(event);

  EXPECT_TRUE(base::ranges::equal(expected_events, events));
}

TEST_F(HnsAdsUserActivityManagerTest, RecordFocusedOnExistingTabEvent) {
  // Arrange
  const UserActivityEventType event_type =
      UserActivityEventType::kTabChangedFocus;

  // Act
  UserActivityManager::GetInstance().RecordEvent(event_type);

  // Assert
  const UserActivityEventList events =
      UserActivityManager::GetInstance().GetHistoryForTimeWindow(
          base::Hours(1));

  UserActivityEventList expected_events;
  UserActivityEventInfo event;
  event.type = event_type;
  event.created_at = Now();
  expected_events.push_back(event);

  EXPECT_TRUE(base::ranges::equal(expected_events, events));
}

TEST_F(HnsAdsUserActivityManagerTest, RecordGeneratedKeywordEvent) {
  // Arrange
  const UserActivityEventType event_type =
      UserActivityEventType::kGeneratedKeyword;

  // Act
  UserActivityManager::GetInstance().RecordEvent(event_type);

  // Assert
  const UserActivityEventList events =
      UserActivityManager::GetInstance().GetHistoryForTimeWindow(
          base::Hours(1));

  UserActivityEventList expected_events;
  UserActivityEventInfo event;
  event.type = event_type;
  event.created_at = Now();
  expected_events.push_back(event);

  EXPECT_TRUE(base::ranges::equal(expected_events, events));
}

TEST_F(HnsAdsUserActivityManagerTest, RecordNewNavigationEvent) {
  // Arrange
  const UserActivityEventType event_type =
      UserActivityEventType::kNewNavigation;

  // Act
  UserActivityManager::GetInstance().RecordEvent(event_type);

  // Assert
  const UserActivityEventList events =
      UserActivityManager::GetInstance().GetHistoryForTimeWindow(
          base::Hours(1));

  UserActivityEventList expected_events;
  UserActivityEventInfo event;
  event.type = event_type;
  event.created_at = Now();
  expected_events.push_back(event);

  EXPECT_TRUE(base::ranges::equal(expected_events, events));
}

TEST_F(HnsAdsUserActivityManagerTest,
       RecordOpenedLinkFromExternalApplicationEvent) {
  // Arrange
  const UserActivityEventType event_type =
      UserActivityEventType::kOpenedLinkFromExternalApplication;

  // Act
  UserActivityManager::GetInstance().RecordEvent(event_type);

  // Assert
  const UserActivityEventList events =
      UserActivityManager::GetInstance().GetHistoryForTimeWindow(
          base::Hours(1));

  UserActivityEventList expected_events;
  UserActivityEventInfo event;
  event.type = event_type;
  event.created_at = Now();
  expected_events.push_back(event);

  EXPECT_TRUE(base::ranges::equal(expected_events, events));
}

TEST_F(HnsAdsUserActivityManagerTest, RecordOpenedNewTabEvent) {
  // Arrange
  const UserActivityEventType event_type = UserActivityEventType::kOpenedNewTab;

  // Act
  UserActivityManager::GetInstance().RecordEvent(event_type);

  // Assert
  const UserActivityEventList events =
      UserActivityManager::GetInstance().GetHistoryForTimeWindow(
          base::Hours(1));

  UserActivityEventList expected_events;
  UserActivityEventInfo event;
  event.type = event_type;
  event.created_at = Now();
  expected_events.push_back(event);

  EXPECT_TRUE(base::ranges::equal(expected_events, events));
}

TEST_F(HnsAdsUserActivityManagerTest, RecordPlayedMediaEvent) {
  // Arrange
  const UserActivityEventType event_type =
      UserActivityEventType::kTabStartedPlayingMedia;

  // Act
  UserActivityManager::GetInstance().RecordEvent(event_type);

  // Assert
  const UserActivityEventList events =
      UserActivityManager::GetInstance().GetHistoryForTimeWindow(
          base::Hours(1));

  UserActivityEventList expected_events;
  UserActivityEventInfo event;
  event.type = event_type;
  event.created_at = Now();
  expected_events.push_back(event);

  EXPECT_TRUE(base::ranges::equal(expected_events, events));
}

TEST_F(HnsAdsUserActivityManagerTest, RecordStoppedPlayingMediaEvent) {
  // Arrange
  const UserActivityEventType event_type =
      UserActivityEventType::kTabStoppedPlayingMedia;

  // Act
  UserActivityManager::GetInstance().RecordEvent(event_type);

  // Assert
  const UserActivityEventList events =
      UserActivityManager::GetInstance().GetHistoryForTimeWindow(
          base::Hours(1));

  UserActivityEventList expected_events;
  UserActivityEventInfo event;
  event.type = event_type;
  event.created_at = Now();
  expected_events.push_back(event);

  EXPECT_TRUE(base::ranges::equal(expected_events, events));
}

TEST_F(HnsAdsUserActivityManagerTest, RecordSubmittedFormEvent) {
  // Arrange
  const UserActivityEventType event_type =
      UserActivityEventType::kSubmittedForm;

  // Act
  UserActivityManager::GetInstance().RecordEvent(event_type);

  // Assert
  const UserActivityEventList events =
      UserActivityManager::GetInstance().GetHistoryForTimeWindow(
          base::Hours(1));

  UserActivityEventList expected_events;
  UserActivityEventInfo event;
  event.type = event_type;
  event.created_at = Now();
  expected_events.push_back(event);

  EXPECT_TRUE(base::ranges::equal(expected_events, events));
}

TEST_F(HnsAdsUserActivityManagerTest, RecordTabUpdatedEvent) {
  // Arrange
  const UserActivityEventType event_type = UserActivityEventType::kTabUpdated;

  // Act
  UserActivityManager::GetInstance().RecordEvent(event_type);

  // Assert
  const UserActivityEventList events =
      UserActivityManager::GetInstance().GetHistoryForTimeWindow(
          base::Hours(1));

  UserActivityEventList expected_events;
  UserActivityEventInfo event;
  event.type = event_type;
  event.created_at = Now();
  expected_events.push_back(event);

  EXPECT_TRUE(base::ranges::equal(expected_events, events));
}

TEST_F(HnsAdsUserActivityManagerTest, RecordTypedAndSelectedNonUrlEvent) {
  // Arrange
  const UserActivityEventType event_type =
      UserActivityEventType::kTypedAndSelectedNonUrl;

  // Act
  UserActivityManager::GetInstance().RecordEvent(event_type);

  // Assert
  const UserActivityEventList events =
      UserActivityManager::GetInstance().GetHistoryForTimeWindow(
          base::Hours(1));

  UserActivityEventList expected_events;
  UserActivityEventInfo event;
  event.type = event_type;
  event.created_at = Now();
  expected_events.push_back(event);

  EXPECT_TRUE(base::ranges::equal(expected_events, events));
}

TEST_F(HnsAdsUserActivityManagerTest,
       RecordTypedKeywordOtherThanDefaultSearchProviderEvent) {
  // Arrange
  const UserActivityEventType event_type =
      UserActivityEventType::kTypedKeywordOtherThanDefaultSearchProvider;

  // Act
  UserActivityManager::GetInstance().RecordEvent(event_type);

  // Assert
  const UserActivityEventList events =
      UserActivityManager::GetInstance().GetHistoryForTimeWindow(
          base::Hours(1));

  UserActivityEventList expected_events;
  UserActivityEventInfo event;
  event.type = event_type;
  event.created_at = Now();
  expected_events.push_back(event);

  EXPECT_TRUE(base::ranges::equal(expected_events, events));
}

TEST_F(HnsAdsUserActivityManagerTest, RecordTypedUrlEvent) {
  // Arrange
  const UserActivityEventType event_type = UserActivityEventType::kTypedUrl;

  // Act
  UserActivityManager::GetInstance().RecordEvent(event_type);

  // Assert
  const UserActivityEventList events =
      UserActivityManager::GetInstance().GetHistoryForTimeWindow(
          base::Hours(1));

  UserActivityEventList expected_events;
  UserActivityEventInfo event;
  event.type = event_type;
  event.created_at = Now();
  expected_events.push_back(event);

  EXPECT_TRUE(base::ranges::equal(expected_events, events));
}

TEST_F(HnsAdsUserActivityManagerTest, RecordUsedAddressBarEvent) {
  // Arrange
  const UserActivityEventType event_type =
      UserActivityEventType::kUsedAddressBar;

  // Act
  UserActivityManager::GetInstance().RecordEvent(event_type);

  // Assert
  const UserActivityEventList events =
      UserActivityManager::GetInstance().GetHistoryForTimeWindow(
          base::Hours(1));

  UserActivityEventList expected_events;
  UserActivityEventInfo event;
  event.type = event_type;
  event.created_at = Now();
  expected_events.push_back(event);

  EXPECT_TRUE(base::ranges::equal(expected_events, events));
}

TEST_F(HnsAdsUserActivityManagerTest, RecordBrowserDidBecomeActiveEvent) {
  // Arrange
  const UserActivityEventType event_type =
      UserActivityEventType::kBrowserDidBecomeActive;

  // Act
  UserActivityManager::GetInstance().RecordEvent(event_type);

  // Assert
  const UserActivityEventList events =
      UserActivityManager::GetInstance().GetHistoryForTimeWindow(
          base::Hours(1));

  UserActivityEventList expected_events;
  UserActivityEventInfo event;
  event.type = event_type;
  event.created_at = Now();
  expected_events.push_back(event);

  EXPECT_TRUE(base::ranges::equal(expected_events, events));
}

TEST_F(HnsAdsUserActivityManagerTest, RecordBrowserDidResignActiveEvent) {
  // Arrange
  const UserActivityEventType event_type =
      UserActivityEventType::kBrowserDidResignActive;

  // Act
  UserActivityManager::GetInstance().RecordEvent(event_type);

  // Assert
  const UserActivityEventList events =
      UserActivityManager::GetInstance().GetHistoryForTimeWindow(
          base::Hours(1));

  UserActivityEventList expected_events;
  UserActivityEventInfo event;
  event.type = event_type;
  event.created_at = Now();
  expected_events.push_back(event);

  EXPECT_TRUE(base::ranges::equal(expected_events, events));
}

TEST_F(HnsAdsUserActivityManagerTest, DoNotRecordEventForNonRewardsUser) {
  // Arrange
  DisableHnsRewardsForTesting();

  const UserActivityEventType event_type =
      UserActivityEventType::kInitializedAds;

  // Act
  UserActivityManager::GetInstance().RecordEvent(event_type);

  // Assert
  const UserActivityEventList events =
      UserActivityManager::GetInstance().GetHistoryForTimeWindow(
          base::Hours(1));

  EXPECT_TRUE(events.empty());
}

TEST_F(HnsAdsUserActivityManagerTest, GetHistoryForTimeWindow) {
  // Arrange
  UserActivityManager::GetInstance().RecordEvent(
      UserActivityEventType::kInitializedAds);
  UserActivityManager::GetInstance().RecordEvent(
      UserActivityEventType::kBrowserDidBecomeActive);
  UserActivityManager::GetInstance().RecordEvent(
      UserActivityEventType::kBrowserDidEnterBackground);
  UserActivityManager::GetInstance().RecordEvent(
      UserActivityEventType::kClickedBackOrForwardNavigationButtons);
  UserActivityManager::GetInstance().RecordEvent(
      UserActivityEventType::kClickedBookmark);
  UserActivityManager::GetInstance().RecordEvent(
      UserActivityEventType::kClickedHomePageButton);
  UserActivityManager::GetInstance().RecordEvent(
      UserActivityEventType::kClickedLink);
  UserActivityManager::GetInstance().RecordEvent(
      UserActivityEventType::kClickedReloadButton);
  UserActivityManager::GetInstance().RecordEvent(
      UserActivityEventType::kClosedTab);
  UserActivityManager::GetInstance().RecordEvent(
      UserActivityEventType::kTabChangedFocus);
  UserActivityManager::GetInstance().RecordEvent(
      UserActivityEventType::kGeneratedKeyword);
  UserActivityManager::GetInstance().RecordEvent(
      UserActivityEventType::kNewNavigation);
  UserActivityManager::GetInstance().RecordEvent(
      UserActivityEventType::kOpenedLinkFromExternalApplication);
  UserActivityManager::GetInstance().RecordEvent(
      UserActivityEventType::kOpenedNewTab);
  UserActivityManager::GetInstance().RecordEvent(
      UserActivityEventType::kTabStartedPlayingMedia);
  UserActivityManager::GetInstance().RecordEvent(
      UserActivityEventType::kTabStoppedPlayingMedia);
  UserActivityManager::GetInstance().RecordEvent(
      UserActivityEventType::kSubmittedForm);

  AdvanceClockBy(base::Hours(1));

  const base::Time time = Now();

  UserActivityManager::GetInstance().RecordEvent(
      UserActivityEventType::kTabUpdated);
  UserActivityManager::GetInstance().RecordEvent(
      UserActivityEventType::kTypedAndSelectedNonUrl);
  UserActivityManager::GetInstance().RecordEvent(
      UserActivityEventType::kTypedKeywordOtherThanDefaultSearchProvider);
  UserActivityManager::GetInstance().RecordEvent(
      UserActivityEventType::kTypedUrl);
  UserActivityManager::GetInstance().RecordEvent(
      UserActivityEventType::kUsedAddressBar);

  AdvanceClockBy(base::Hours(1));

  // Act
  const UserActivityEventList events =
      UserActivityManager::GetInstance().GetHistoryForTimeWindow(
          base::Hours(1));

  // Assert
  UserActivityEventList expected_events;
  UserActivityEventInfo event;
  event.type = UserActivityEventType::kTabUpdated;
  event.created_at = time;
  expected_events.push_back(event);
  event.type = UserActivityEventType::kTypedAndSelectedNonUrl;
  event.created_at = time;
  expected_events.push_back(event);
  event.type =
      UserActivityEventType::kTypedKeywordOtherThanDefaultSearchProvider;
  event.created_at = time;
  expected_events.push_back(event);
  event.type = UserActivityEventType::kTypedUrl;
  event.created_at = time;
  expected_events.push_back(event);
  event.type = UserActivityEventType::kUsedAddressBar;
  event.created_at = time;
  expected_events.push_back(event);

  EXPECT_TRUE(base::ranges::equal(expected_events, events));
}

TEST_F(HnsAdsUserActivityManagerTest, MaximumHistoryItems) {
  // Arrange
  const UserActivityEventType event_type = UserActivityEventType::kClosedTab;
  for (int i = 0; i < kMaximumHistoryItems; i++) {
    UserActivityManager::GetInstance().RecordEvent(event_type);
  }

  // Act
  UserActivityManager::GetInstance().RecordEvent(
      UserActivityEventType::kOpenedNewTab);

  const UserActivityEventList events =
      UserActivityManager::GetInstance().GetHistoryForTimeWindow(
          base::Hours(1));

  // Assert
  UserActivityEventList expected_events;
  UserActivityEventInfo event;

  for (int i = 0; i < kMaximumHistoryItems - 1; i++) {
    event.type = event_type;
    event.created_at = Now();
    expected_events.push_back(event);
  }

  event.type = UserActivityEventType::kOpenedNewTab;
  event.created_at = Now();
  expected_events.push_back(event);

  EXPECT_TRUE(base::ranges::equal(expected_events, events));
}

}  // namespace hns_ads
