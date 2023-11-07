/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/conversions/queue/queue_item/conversion_queue_item_builder.h"

#include "hns/components/hns_ads/core/internal/ads/ad_unittest_util.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_base.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_time_util.h"
#include "hns/components/hns_ads/core/internal/conversions/conversion/conversion_builder.h"
#include "hns/components/hns_ads/core/internal/conversions/types/verifiable_conversion/verifiable_conversion_unittest_constants.h"
#include "hns/components/hns_ads/core/internal/user_interaction/ad_events/ad_event_builder.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads {

class HnsAdsConversionQueueItemBuilderTest : public UnitTestBase {};

TEST_F(HnsAdsConversionQueueItemBuilderTest, BuildConversionQueueItem) {
  // Arrange
  const AdInfo ad = BuildAdForTesting(AdType::kNotificationAd,
                                      /*should_use_random_uuids*/ true);

  const ConversionInfo conversion =
      BuildConversion(BuildAdEvent(ad, ConfirmationType::kViewed,
                                   /*created_at*/ Now()),
                      /*verifiable_conversion*/ absl::nullopt);

  // Act
  const ConversionQueueItemInfo conversion_queue_item =
      BuildConversionQueueItem(conversion, /*process_at*/ Now());

  // Assert
  ConversionQueueItemInfo expected_conversion_queue_item;
  expected_conversion_queue_item.conversion = conversion;
  expected_conversion_queue_item.process_at = Now();
  expected_conversion_queue_item.was_processed = false;

  EXPECT_EQ(expected_conversion_queue_item, conversion_queue_item);
}

TEST_F(HnsAdsConversionQueueItemBuilderTest,
       BuildVerifiableConversionQueueItem) {
  // Arrange
  const AdInfo ad = BuildAdForTesting(AdType::kNotificationAd,
                                      /*should_use_random_uuids*/ true);

  const ConversionInfo conversion = BuildConversion(
      BuildAdEvent(ad, ConfirmationType::kViewed,
                   /*created_at*/ Now()),
      VerifiableConversionInfo{kVerifiableConversionId,
                               kVerifiableConversionAdvertiserPublicKey});

  // Act
  const ConversionQueueItemInfo conversion_queue_item =
      BuildConversionQueueItem(conversion, /*process_at*/ Now());

  // Assert
  ConversionQueueItemInfo expected_conversion_queue_item;
  expected_conversion_queue_item.conversion = conversion;
  expected_conversion_queue_item.process_at = Now();
  expected_conversion_queue_item.was_processed = false;

  EXPECT_EQ(expected_conversion_queue_item, conversion_queue_item);
}

}  // namespace hns_ads
