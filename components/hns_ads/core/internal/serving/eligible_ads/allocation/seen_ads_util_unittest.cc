/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/serving/eligible_ads/allocation/seen_ads_util.h"

#include "hns/components/hns_ads/core/internal/common/unittest/unittest_time_util.h"
#include "hns/components/hns_ads/core/internal/creatives/notification_ads/creative_notification_ad_info.h"
#include "hns/components/hns_ads/core/internal/creatives/notification_ads/creative_notification_ad_unittest_util.h"
#include "hns/components/hns_ads/core/internal/user_interaction/ad_events/ad_event_unittest_util.h"
#include "testing/gtest/include/gtest/gtest.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads {

TEST(HnsAdsAdEventUtilTest, DoNotGetLastSeenAdAtForEmptyAdEvents) {
  // Arrange
  const CreativeNotificationAdInfo creative_ad =
      BuildCreativeNotificationAdForTesting(/*should_use_random_uuids*/ true);

  // Act

  // Assert
  EXPECT_FALSE(GetLastSeenAdAt(/*ad_events*/ {}, creative_ad));
}

TEST(HnsAdsAdEventUtilTest, DoNotGetLastSeenAdAtForUnseenAd) {
  // Arrange
  const CreativeNotificationAdInfo creative_ad_1 =
      BuildCreativeNotificationAdForTesting(/*should_use_random_uuids*/ true);

  AdEventList ad_events;
  const AdEventInfo ad_event = BuildAdEventForTesting(
      creative_ad_1, AdType::kNotificationAd, ConfirmationType::kViewed,
      /*created_at*/ Now() - base::Hours(12));
  ad_events.push_back(ad_event);

  const CreativeNotificationAdInfo creative_ad_2 =
      BuildCreativeNotificationAdForTesting(/*should_use_random_uuids*/ true);

  // Act

  // Assert
  EXPECT_FALSE(GetLastSeenAdAt(ad_events, creative_ad_2));
}

TEST(HnsAdsAdEventUtilTest, GetLastSeenAdAt) {
  // Arrange
  AdEventList ad_events;

  const CreativeNotificationAdInfo creative_ad_1 =
      BuildCreativeNotificationAdForTesting(/*should_use_random_uuids*/ true);

  const CreativeNotificationAdInfo creative_ad_2 =
      BuildCreativeNotificationAdForTesting(/*should_use_random_uuids*/ true);

  const base::Time now = Now();

  const AdEventInfo ad_event_4 = BuildAdEventForTesting(
      creative_ad_1, AdType::kNotificationAd, ConfirmationType::kConversion,
      now - base::Hours(3));
  ad_events.push_back(ad_event_4);

  const AdEventInfo ad_event_3 =
      BuildAdEventForTesting(creative_ad_1, AdType::kNotificationAd,
                             ConfirmationType::kViewed, now - base::Hours(6));
  ad_events.push_back(ad_event_3);

  const AdEventInfo ad_event_2 =
      BuildAdEventForTesting(creative_ad_2, AdType::kNotificationAd,
                             ConfirmationType::kViewed, now - base::Hours(11));
  ad_events.push_back(ad_event_2);

  const AdEventInfo ad_event_1 =
      BuildAdEventForTesting(creative_ad_1, AdType::kNotificationAd,
                             ConfirmationType::kViewed, now - base::Hours(12));
  ad_events.push_back(ad_event_1);

  // Act

  // Assert
  EXPECT_EQ(now - base::Hours(6), GetLastSeenAdAt(ad_events, creative_ad_1));
}

}  // namespace hns_ads
