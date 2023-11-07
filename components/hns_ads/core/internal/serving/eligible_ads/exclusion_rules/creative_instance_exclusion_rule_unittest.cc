/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/serving/eligible_ads/exclusion_rules/creative_instance_exclusion_rule.h"

#include "hns/components/hns_ads/core/internal/ads/ad_unittest_constants.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_base.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_time_util.h"
#include "hns/components/hns_ads/core/internal/creatives/creative_ad_info.h"
#include "hns/components/hns_ads/core/internal/user_interaction/ad_events/ad_event_unittest_util.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads {

class HnsAdsCreativeInstanceExclusionRuleTest : public UnitTestBase {};

TEST_F(HnsAdsCreativeInstanceExclusionRuleTest,
       ShouldIncludeIfThereAreNoAdEvents) {
  // Arrange
  CreativeAdInfo creative_ad;
  creative_ad.creative_instance_id = kCreativeInstanceId;

  const CreativeInstanceExclusionRule exclusion_rule(/*ad_events*/ {});

  // Act

  // Assert
  EXPECT_TRUE(exclusion_rule.ShouldInclude(creative_ad).has_value());
}

TEST_F(HnsAdsCreativeInstanceExclusionRuleTest, AdAllowedAfter1Hour) {
  // Arrange
  CreativeAdInfo creative_ad;
  creative_ad.creative_instance_id = kCreativeInstanceId;

  AdEventList ad_events;
  const AdEventInfo ad_event = BuildAdEventForTesting(
      creative_ad, AdType::kNotificationAd, ConfirmationType::kServed, Now());
  ad_events.push_back(ad_event);

  const CreativeInstanceExclusionRule exclusion_rule(ad_events);

  AdvanceClockBy(base::Hours(1));

  // Act

  // Assert
  EXPECT_TRUE(exclusion_rule.ShouldInclude(creative_ad).has_value());
}

TEST_F(HnsAdsCreativeInstanceExclusionRuleTest,
       AdAllowedAfter1HourForMultipleTypes) {
  // Arrange
  CreativeAdInfo creative_ad;
  creative_ad.creative_instance_id = kCreativeInstanceId;

  AdEventList ad_events;

  const AdEventInfo ad_event_1 = BuildAdEventForTesting(
      creative_ad, AdType::kNotificationAd, ConfirmationType::kServed, Now());
  ad_events.push_back(ad_event_1);

  const AdEventInfo ad_event_2 = BuildAdEventForTesting(
      creative_ad, AdType::kNewTabPageAd, ConfirmationType::kServed, Now());
  ad_events.push_back(ad_event_2);

  const AdEventInfo ad_event_3 =
      BuildAdEventForTesting(creative_ad, AdType::kPromotedContentAd,
                             ConfirmationType::kServed, Now());
  ad_events.push_back(ad_event_3);

  const AdEventInfo ad_event_4 = BuildAdEventForTesting(
      creative_ad, AdType::kSearchResultAd, ConfirmationType::kServed, Now());
  ad_events.push_back(ad_event_4);

  const CreativeInstanceExclusionRule exclusion_rule(ad_events);

  AdvanceClockBy(base::Hours(1));

  // Act

  // Assert
  EXPECT_TRUE(exclusion_rule.ShouldInclude(creative_ad).has_value());
}

TEST_F(HnsAdsCreativeInstanceExclusionRuleTest,
       DoNotAllowTheSameAdWithin1Hour) {
  // Arrange
  CreativeAdInfo creative_ad;
  creative_ad.creative_instance_id = kCreativeInstanceId;

  AdEventList ad_events;
  const AdEventInfo ad_event = BuildAdEventForTesting(
      creative_ad, AdType::kNotificationAd, ConfirmationType::kServed, Now());
  ad_events.push_back(ad_event);

  const CreativeInstanceExclusionRule exclusion_rule(ad_events);

  AdvanceClockBy(base::Hours(1) - base::Milliseconds(1));

  // Act

  // Assert
  EXPECT_FALSE(exclusion_rule.ShouldInclude(creative_ad).has_value());
}

}  // namespace hns_ads
