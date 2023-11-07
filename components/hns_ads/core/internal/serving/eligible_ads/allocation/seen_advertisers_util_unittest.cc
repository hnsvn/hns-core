/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/serving/eligible_ads/allocation/seen_advertisers_util.h"

#include <string>

#include "base/uuid.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_time_util.h"
#include "hns/components/hns_ads/core/internal/creatives/notification_ads/creative_notification_ad_info.h"
#include "hns/components/hns_ads/core/internal/creatives/notification_ads/creative_notification_ad_unittest_util.h"
#include "hns/components/hns_ads/core/internal/user_interaction/ad_events/ad_event_unittest_util.h"
#include "testing/gtest/include/gtest/gtest.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads {

TEST(HnsAdsSeenAdvertisersUtilTest,
     DoNotGetLastSeenAdvertiserAtForEmptyAdEvents) {
  // Arrange
  const CreativeNotificationAdInfo creative_ad =
      BuildCreativeNotificationAdForTesting(/*should_use_random_uuids*/ true);

  // Act

  // Assert
  EXPECT_FALSE(GetLastSeenAdvertiserAt(/*ad_events*/ {}, creative_ad));
}

TEST(HnsAdsSeenAdvertisersUtilTest,
     DoNotGetLastSeenAdvertiserAtForUnseenAdvertiser) {
  // Arrange
  const CreativeNotificationAdInfo creative_ad_1 =
      BuildCreativeNotificationAdForTesting(/*should_use_random_uuids*/ true);

  AdEventList ad_events;
  const AdEventInfo ad_event = BuildAdEventForTesting(
      creative_ad_1, AdType::kNotificationAd, ConfirmationType::kViewed,
      Now() - base::Hours(12));
  ad_events.push_back(ad_event);

  const CreativeNotificationAdInfo creative_ad_2 =
      BuildCreativeNotificationAdForTesting(/*should_use_random_uuids*/ true);

  // Act

  // Assert
  EXPECT_FALSE(GetLastSeenAdvertiserAt(ad_events, creative_ad_2));
}

TEST(HnsAdsSeenAdvertisersUtilTest, GetLastSeenAdvertiserAt) {
  // Arrange
  const std::string advertiser_id_1 =
      base::Uuid::GenerateRandomV4().AsLowercaseString();
  const std::string advertiser_id_2 =
      base::Uuid::GenerateRandomV4().AsLowercaseString();

  CreativeNotificationAdInfo creative_ad_1 =
      BuildCreativeNotificationAdForTesting(/*should_use_random_uuids*/ true);
  creative_ad_1.advertiser_id = advertiser_id_1;

  CreativeNotificationAdInfo creative_ad_2 =
      BuildCreativeNotificationAdForTesting(/*should_use_random_uuids*/ true);
  creative_ad_2.advertiser_id = advertiser_id_2;

  CreativeNotificationAdInfo creative_ad_3 =
      BuildCreativeNotificationAdForTesting(/*should_use_random_uuids*/ true);
  creative_ad_3.advertiser_id = advertiser_id_1;

  AdEventList ad_events;

  const base::Time now = Now();

  const AdEventInfo ad_event_4 =
      BuildAdEventForTesting(creative_ad_1, AdType::kNotificationAd,
                             ConfirmationType::kViewed, now - base::Hours(3));
  ad_events.push_back(ad_event_4);

  const AdEventInfo ad_event_3 =
      BuildAdEventForTesting(creative_ad_3, AdType::kNotificationAd,
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
  EXPECT_EQ(now - base::Hours(3),
            GetLastSeenAdvertiserAt(ad_events, creative_ad_3));
}

}  // namespace hns_ads
