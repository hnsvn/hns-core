/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/conversions/queue/queue_item/conversion_queue_item_validation_util.h"

#include "hns/components/hns_ads/core/internal/ads/ad_unittest_util.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_time_util.h"
#include "hns/components/hns_ads/core/internal/conversions/conversion/conversion_builder.h"
#include "hns/components/hns_ads/core/internal/conversions/conversion/conversion_info.h"
#include "hns/components/hns_ads/core/internal/conversions/queue/queue_item/conversion_queue_item_unittest_util.h"
#include "hns/components/hns_ads/core/internal/conversions/types/verifiable_conversion/verifiable_conversion_unittest_constants.h"
#include "hns/components/hns_ads/core/internal/user_interaction/ad_events/ad_event_builder.h"
#include "hns/components/hns_ads/core/internal/user_interaction/ad_events/ad_event_info.h"
#include "hns/components/hns_ads/core/public/ad_info.h"
#include "hns/components/hns_ads/core/public/ad_type.h"
#include "testing/gtest/include/gtest/gtest.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads {

TEST(HnsAdsValidationUtilTest, InvalidConversionQueueItem) {
  // Arrange
  const AdInfo ad = BuildAdForTesting(AdType::kNotificationAd,
                                      /*should_use_random_uuids*/ true);
  ConversionInfo conversion = BuildConversion(
      BuildAdEvent(ad, ConfirmationType::kViewed, /*created_at*/ Now()),
      VerifiableConversionInfo{kVerifiableConversionId,
                               kVerifiableConversionAdvertiserPublicKey});
  conversion.ad_type = AdType::kUndefined;

  ConversionQueueItemList conversion_queue_items =
      BuildConversionQueueItemsForTesting(conversion, /*count*/ 1);
  conversion_queue_items[0].process_at = base::Time();

  // Act

  // Assert
  EXPECT_EQ("ad_type,process_at", GetConversionQueueItemInvalidFieldsNames(
                                      conversion_queue_items[0]));
}

TEST(HnsAdsValidationUtilTest, ValidConversionQueueItem) {
  // Arrange
  const AdInfo ad = BuildAdForTesting(AdType::kNotificationAd,
                                      /*should_use_random_uuids*/ true);
  const ConversionInfo conversion = BuildConversion(
      BuildAdEvent(ad, ConfirmationType::kViewed, /*created_at*/ Now()),
      VerifiableConversionInfo{kVerifiableConversionId,
                               kVerifiableConversionAdvertiserPublicKey});
  const ConversionQueueItemList conversion_queue_items =
      BuildConversionQueueItemsForTesting(conversion, /*count*/ 1);

  // Act

  // Assert
  EXPECT_EQ(
      "", GetConversionQueueItemInvalidFieldsNames(conversion_queue_items[0]));
}

}  // namespace hns_ads
