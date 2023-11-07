/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/user_interaction/ad_events/ad_event_handler_util.h"

#include "hns/components/hns_ads/core/internal/ads/ad_unittest_util.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_time_util.h"
#include "hns/components/hns_ads/core/internal/user_interaction/ad_events/ad_event_builder.h"
#include "hns/components/hns_ads/core/internal/user_interaction/ad_events/ad_event_info.h"
#include "hns/components/hns_ads/core/public/ad_info.h"
#include "hns/components/hns_ads/core/public/confirmation_type.h"
#include "testing/gtest/include/gtest/gtest.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads {

TEST(HnsAdsAdEventHandlerUtilTest, HasFiredAdEvent) {
  // Arrange
  const AdInfo ad = BuildAdForTesting(AdType::kNotificationAd,
                                      /*should_use_random_uuids*/ true);

  AdEventList ad_events;
  const AdEventInfo ad_event = BuildAdEvent(ad, ConfirmationType::kServed,
                                            /*created_at*/ Now());
  ad_events.push_back(ad_event);

  // Act

  // Assert
  EXPECT_TRUE(HasFiredAdEvent(ad, ad_events, ConfirmationType::kServed));
}

TEST(HnsAdsAdEventHandlerUtilTest, HasNotFiredAdEvent) {
  // Arrange
  const AdInfo ad = BuildAdForTesting(AdType::kNotificationAd,
                                      /*should_use_random_uuids*/ true);

  AdEventList ad_events;
  const AdEventInfo ad_event =
      BuildAdEvent(ad, ConfirmationType::kServed, /*created_at*/ Now());
  ad_events.push_back(ad_event);

  // Act

  // Assert
  EXPECT_FALSE(HasFiredAdEvent(ad, ad_events, ConfirmationType::kViewed));
}

}  // namespace hns_ads
