/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/conversions/queue/queue_item/conversion_queue_item_util.h"

#include "base/time/time.h"
#include "hns/components/hns_ads/core/internal/ads/ad_unittest_util.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_base.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_time_util.h"
#include "hns/components/hns_ads/core/internal/conversions/conversion/conversion_builder.h"
#include "hns/components/hns_ads/core/internal/conversions/queue/queue_item/conversion_queue_item_builder.h"
#include "hns/components/hns_ads/core/internal/conversions/queue/queue_item/conversion_queue_item_util_constants.h"
#include "hns/components/hns_ads/core/internal/user_interaction/ad_events/ad_event_builder.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads {

class HnsAdsConversionQueueItemDelayTest : public UnitTestBase {};

TEST_F(HnsAdsConversionQueueItemDelayTest,
       CalculateDelayBeforeProcessingConversionQueueItem) {
  // Arrange
  const AdInfo ad = BuildAdForTesting(AdType::kNotificationAd,
                                      /*should_use_random_uuids*/ true);
  const ConversionInfo conversion =
      BuildConversion(BuildAdEvent(ad, ConfirmationType::kViewed,
                                   /*created_at*/ Now()),
                      /*verifiable_conversion*/ absl::nullopt);
  const ConversionQueueItemInfo conversion_queue_item =
      BuildConversionQueueItem(conversion,
                               /*process_at*/ Now() + base::Hours(1));

  // Act
  const base::TimeDelta delay =
      CalculateDelayBeforeProcessingConversionQueueItem(conversion_queue_item);

  // Assert
  EXPECT_EQ(base::Hours(1), delay);
}

TEST_F(HnsAdsConversionQueueItemDelayTest,
       CalculateDelayBeforeProcessingPastDueConversionQueueItem) {
  // Arrange
  const AdInfo ad = BuildAdForTesting(AdType::kNotificationAd,
                                      /*should_use_random_uuids*/ true);
  const ConversionInfo conversion =
      BuildConversion(BuildAdEvent(ad, ConfirmationType::kViewed,
                                   /*created_at*/ DistantPast()),
                      /*verifiable_conversion*/ absl::nullopt);
  const ConversionQueueItemInfo conversion_queue_item =
      BuildConversionQueueItem(conversion, /*process_at*/ DistantPast());

  // Act
  const base::TimeDelta delay =
      CalculateDelayBeforeProcessingConversionQueueItem(conversion_queue_item);

  // Assert
  EXPECT_EQ(kMinimumDelayBeforeProcessingQueueItem, delay);
}

TEST_F(HnsAdsConversionQueueItemDelayTest,
       CalculateMinimumDelayBeforeProcessingConversionQueueItem) {
  // Arrange
  const AdInfo ad = BuildAdForTesting(AdType::kNotificationAd,
                                      /*should_use_random_uuids*/ true);
  const ConversionInfo conversion =
      BuildConversion(BuildAdEvent(ad, ConfirmationType::kViewed,
                                   /*created_at*/ Now()),
                      /*verifiable_conversion*/ absl::nullopt);
  const ConversionQueueItemInfo conversion_queue_item =
      BuildConversionQueueItem(conversion,
                               /*process_at*/ Now() + base::Milliseconds(1));

  // Act
  const base::TimeDelta delay =
      CalculateDelayBeforeProcessingConversionQueueItem(conversion_queue_item);

  // Assert
  EXPECT_EQ(kMinimumDelayBeforeProcessingQueueItem, delay);
}

}  // namespace hns_ads
