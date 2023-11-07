/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/conversions/conversion/conversion_builder.h"

#include "hns/components/hns_ads/core/internal/ads/ad_unittest_constants.h"
#include "hns/components/hns_ads/core/internal/ads/ad_unittest_util.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_time_util.h"
#include "hns/components/hns_ads/core/internal/conversions/actions/conversion_action_types.h"
#include "hns/components/hns_ads/core/internal/conversions/conversion/conversion_info.h"
#include "hns/components/hns_ads/core/internal/conversions/types/verifiable_conversion/verifiable_conversion_info.h"
#include "hns/components/hns_ads/core/internal/conversions/types/verifiable_conversion/verifiable_conversion_unittest_constants.h"
#include "hns/components/hns_ads/core/internal/user_interaction/ad_events/ad_event_builder.h"
#include "hns/components/hns_ads/core/internal/user_interaction/ad_events/ad_event_info.h"
#include "hns/components/hns_ads/core/public/ad_info.h"
#include "testing/gtest/include/gtest/gtest.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads {

TEST(HnsAdsConversionBuilderTest, BuildConversion) {
  // Arrange
  const AdInfo ad = BuildAdForTesting(AdType::kNotificationAd,
                                      /*should_use_random_uuids*/ false);

  // Act
  const ConversionInfo conversion =
      BuildConversion(BuildAdEvent(ad, ConfirmationType::kViewed,
                                   /*created_at*/ Now()),
                      /*verifiable_conversion*/ absl::nullopt);

  // Assert
  ConversionInfo expected_conversion;
  expected_conversion.ad_type = AdType::kNotificationAd;
  expected_conversion.creative_instance_id = kCreativeInstanceId;
  expected_conversion.creative_set_id = kCreativeSetId;
  expected_conversion.campaign_id = kCampaignId;
  expected_conversion.advertiser_id = kAdvertiserId;
  expected_conversion.segment = kSegment;
  expected_conversion.action_type = ConversionActionType::kViewThrough;

  EXPECT_EQ(expected_conversion, conversion);
}

TEST(HnsAdsConversionBuilderTest, BuildVerifiableConversion) {
  // Arrange
  const AdInfo ad = BuildAdForTesting(AdType::kNotificationAd,
                                      /*should_use_random_uuids*/ false);

  // Act
  const ConversionInfo conversion = BuildConversion(
      BuildAdEvent(ad, ConfirmationType::kViewed,
                   /*created_at*/ Now()),
      VerifiableConversionInfo{kVerifiableConversionId,
                               kVerifiableConversionAdvertiserPublicKey});

  // Assert
  ConversionInfo expected_conversion;
  expected_conversion.ad_type = AdType::kNotificationAd;
  expected_conversion.creative_instance_id = kCreativeInstanceId;
  expected_conversion.creative_set_id = kCreativeSetId;
  expected_conversion.campaign_id = kCampaignId;
  expected_conversion.advertiser_id = kAdvertiserId;
  expected_conversion.segment = kSegment;
  expected_conversion.action_type = ConversionActionType::kViewThrough;
  expected_conversion.verifiable = VerifiableConversionInfo{
      kVerifiableConversionId, kVerifiableConversionAdvertiserPublicKey};

  EXPECT_EQ(expected_conversion, conversion);
}

}  // namespace hns_ads
