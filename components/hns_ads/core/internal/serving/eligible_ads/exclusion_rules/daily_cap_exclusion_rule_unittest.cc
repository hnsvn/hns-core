/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/serving/eligible_ads/exclusion_rules/daily_cap_exclusion_rule.h"

#include "hns/components/hns_ads/core/internal/common/unittest/unittest_base.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_time_util.h"
#include "hns/components/hns_ads/core/internal/creatives/creative_ad_info.h"
#include "hns/components/hns_ads/core/internal/user_interaction/ad_events/ad_event_unittest_util.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads {

namespace {

constexpr const char* kCampaignIds[] = {"60267cee-d5bb-4a0d-baaf-91cd7f18e07e",
                                        "90762cee-d5bb-4a0d-baaf-61cd7f18e07e"};

}  // namespace

class HnsAdsDailyCapExclusionRuleTest : public UnitTestBase {};

TEST_F(HnsAdsDailyCapExclusionRuleTest, ShouldIncludeIfThereAreNoAdEvents) {
  // Arrange
  CreativeAdInfo creative_ad;
  creative_ad.campaign_id = kCampaignIds[0];
  creative_ad.daily_cap = 2;

  const DailyCapExclusionRule exclusion_rule(/*ad_events*/ {});

  // Act

  // Assert
  EXPECT_TRUE(exclusion_rule.ShouldInclude(creative_ad).has_value());
}

TEST_F(HnsAdsDailyCapExclusionRuleTest, ShouldIncludeIfDoesNotExceedCap) {
  // Arrange
  CreativeAdInfo creative_ad;
  creative_ad.campaign_id = kCampaignIds[0];
  creative_ad.daily_cap = 2;

  AdEventList ad_events;
  const AdEventInfo ad_event = BuildAdEventForTesting(
      creative_ad, AdType::kNotificationAd, ConfirmationType::kServed, Now());
  ad_events.push_back(ad_event);

  const DailyCapExclusionRule exclusion_rule(ad_events);

  // Act

  // Assert
  EXPECT_TRUE(exclusion_rule.ShouldInclude(creative_ad).has_value());
}

TEST_F(HnsAdsDailyCapExclusionRuleTest,
       ShouldIncludeIfDoesNotExceedCapForNoMatchingCampaigns) {
  // Arrange
  CreativeAdInfo creative_ad_1;
  creative_ad_1.campaign_id = kCampaignIds[0];
  creative_ad_1.daily_cap = 1;

  CreativeAdInfo creative_ad_2;
  creative_ad_2.campaign_id = kCampaignIds[1];

  AdEventList ad_events;
  const AdEventInfo ad_event = BuildAdEventForTesting(
      creative_ad_2, AdType::kNotificationAd, ConfirmationType::kServed, Now());
  ad_events.push_back(ad_event);

  const DailyCapExclusionRule exclusion_rule(ad_events);

  // Act

  // Assert
  EXPECT_TRUE(exclusion_rule.ShouldInclude(creative_ad_1).has_value());
}

TEST_F(HnsAdsDailyCapExclusionRuleTest,
       ShouldIncludeIfDoesNotExceedCapWithin1Day) {
  // Arrange
  CreativeAdInfo creative_ad;
  creative_ad.campaign_id = kCampaignIds[0];
  creative_ad.daily_cap = 2;

  AdEventList ad_events;
  const AdEventInfo ad_event = BuildAdEventForTesting(
      creative_ad, AdType::kNotificationAd, ConfirmationType::kServed, Now());
  ad_events.push_back(ad_event);

  const DailyCapExclusionRule exclusion_rule(ad_events);

  AdvanceClockBy(base::Days(1) - base::Milliseconds(1));

  // Act

  // Assert
  EXPECT_TRUE(exclusion_rule.ShouldInclude(creative_ad).has_value());
}

TEST_F(HnsAdsDailyCapExclusionRuleTest,
       ShouldIncludeIfDoesNotExceedCapAfter1Day) {
  // Arrange
  CreativeAdInfo creative_ad;
  creative_ad.campaign_id = kCampaignIds[0];
  creative_ad.daily_cap = 2;

  AdEventList ad_events;
  const AdEventInfo ad_event = BuildAdEventForTesting(
      creative_ad, AdType::kNotificationAd, ConfirmationType::kServed, Now());
  ad_events.push_back(ad_event);

  const DailyCapExclusionRule exclusion_rule(ad_events);

  AdvanceClockBy(base::Days(1));

  // Act

  // Assert
  EXPECT_TRUE(exclusion_rule.ShouldInclude(creative_ad).has_value());
}

TEST_F(HnsAdsDailyCapExclusionRuleTest, ShouldExcludeIfExceedsCap) {
  // Arrange
  CreativeAdInfo creative_ad;
  creative_ad.campaign_id = kCampaignIds[0];
  creative_ad.daily_cap = 2;

  AdEventList ad_events;
  const AdEventInfo ad_event = BuildAdEventForTesting(
      creative_ad, AdType::kNotificationAd, ConfirmationType::kServed, Now());
  ad_events.push_back(ad_event);
  ad_events.push_back(ad_event);

  const DailyCapExclusionRule exclusion_rule(ad_events);

  // Act

  // Assert
  EXPECT_FALSE(exclusion_rule.ShouldInclude(creative_ad).has_value());
}

}  // namespace hns_ads
