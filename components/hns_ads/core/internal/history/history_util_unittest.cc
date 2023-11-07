/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/history/history_util.h"

#include "base/ranges/algorithm.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_base.h"
#include "hns/components/hns_ads/core/internal/creatives/notification_ads/creative_notification_ad_unittest_util.h"
#include "hns/components/hns_ads/core/internal/creatives/notification_ads/notification_ad_builder.h"
#include "hns/components/hns_ads/core/internal/history/history_constants.h"
#include "hns/components/hns_ads/core/internal/history/history_manager.h"
#include "hns/components/hns_ads/core/public/ads/notification_ad_info.h"
#include "hns/components/hns_ads/core/public/confirmation_type.h"
#include "hns/components/hns_ads/core/public/history/history_item_info.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads {

namespace {

HistoryItemInfo BuildAndAddHistoryItem() {
  const CreativeNotificationAdInfo creative_ad =
      BuildCreativeNotificationAdForTesting(/*should_use_random_uuids*/ true);
  const NotificationAdInfo ad = BuildNotificationAd(creative_ad);

  return AddHistory(ad, ConfirmationType::kViewed, ad.title, ad.body);
}

}  // namespace

class HnsAdsHistoryUtilTest : public UnitTestBase {};

TEST_F(HnsAdsHistoryUtilTest, AddHistory) {
  // Arrange

  // Act
  const HistoryItemInfo history_item = BuildAndAddHistoryItem();

  // Assert
  const HistoryItemList expected_history = {history_item};
  EXPECT_TRUE(base::ranges::equal(expected_history, HistoryManager::Get()));
}

TEST_F(HnsAdsHistoryUtilTest, PurgeHistoryOlderThanTimeWindow) {
  // Arrange
  BuildAndAddHistoryItem();

  AdvanceClockBy(kHistoryTimeWindow + base::Milliseconds(1));

  // Act
  const HistoryItemInfo history_item = BuildAndAddHistoryItem();

  // Assert
  const HistoryItemList expected_history = {history_item};
  EXPECT_TRUE(base::ranges::equal(expected_history, HistoryManager::Get()));
}

TEST_F(HnsAdsHistoryUtilTest, DoNotPurgeHistoryWithinTimeWindow) {
  // Arrange
  const HistoryItemInfo history_item_1 = BuildAndAddHistoryItem();

  AdvanceClockBy(kHistoryTimeWindow);

  // Act
  const HistoryItemInfo history_item_2 = BuildAndAddHistoryItem();

  // Assert
  const HistoryItemList expected_history = {history_item_2, history_item_1};
  EXPECT_TRUE(base::ranges::equal(expected_history, HistoryManager::Get()));
}

}  // namespace hns_ads
