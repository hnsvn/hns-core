/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/public/ads/notification_ad_info.h"

#include "hns/components/hns_ads/core/internal/common/unittest/unittest_base.h"
#include "hns/components/hns_ads/core/internal/creatives/notification_ads/creative_notification_ad_info.h"
#include "hns/components/hns_ads/core/internal/creatives/notification_ads/creative_notification_ad_unittest_util.h"
#include "hns/components/hns_ads/core/internal/creatives/notification_ads/notification_ad_builder.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads {

class HnsAdsNotificationAdInfoTest : public UnitTestBase {};

TEST_F(HnsAdsNotificationAdInfoTest, IsValid) {
  // Arrange
  const CreativeNotificationAdInfo creative_ad =
      BuildCreativeNotificationAdForTesting(/*should_use_random_uuids*/ true);
  const NotificationAdInfo ad = BuildNotificationAd(creative_ad);

  // Act

  // Assert
  EXPECT_TRUE(ad.IsValid());
}

TEST_F(HnsAdsNotificationAdInfoTest, IsInvalid) {
  // Arrange
  const NotificationAdInfo ad;

  // Act

  // Assert
  EXPECT_FALSE(ad.IsValid());
}

}  // namespace hns_ads
