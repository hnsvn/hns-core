/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/serving/eligible_ads/exclusion_rules/total_max_exclusion_rule.h"

#include "hns/components/hns_ads/core/internal/common/unittest/unittest_base.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_time_util.h"
#include "hns/components/hns_ads/core/internal/creatives/creative_ad_info.h"
#include "hns/components/hns_ads/core/internal/user_interaction/ad_events/ad_event_unittest_util.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads {

namespace {

constexpr const char* kCreativeSetIds[] = {
    "654f10df-fbc4-4a92-8d43-2edf73734a60",
    "465f10df-fbc4-4a92-8d43-4edf73734a60"};

}  // namespace

class HnsAdsTotalMaxExclusionRuleTest : public UnitTestBase {};

TEST_F(HnsAdsTotalMaxExclusionRuleTest, ShouldIncludeIfThereAreNoAdEvents) {
  // Arrange
  CreativeAdInfo creative_ad;
  creative_ad.creative_set_id = kCreativeSetIds[0];
  creative_ad.total_max = 2;

  const TotalMaxExclusionRule exclusion_rule(/*ad_events*/ {});

  // Act

  // Assert
  EXPECT_TRUE(exclusion_rule.ShouldInclude(creative_ad).has_value());
}

TEST_F(HnsAdsTotalMaxExclusionRuleTest, ShouldIncludeIfDoesNotExceedCap) {
  // Arrange
  CreativeAdInfo creative_ad;
  creative_ad.creative_set_id = kCreativeSetIds[0];
  creative_ad.total_max = 2;

  AdEventList ad_events;
  const AdEventInfo ad_event = BuildAdEventForTesting(
      creative_ad, AdType::kNotificationAd, ConfirmationType::kServed, Now());
  ad_events.push_back(ad_event);

  const TotalMaxExclusionRule exclusion_rule(ad_events);

  // Act

  // Assert
  EXPECT_TRUE(exclusion_rule.ShouldInclude(creative_ad).has_value());
}

TEST_F(HnsAdsTotalMaxExclusionRuleTest,
       ShouldIncludeIfDoesNotExceedCapForNoMatchingCreatives) {
  // Arrange
  CreativeAdInfo creative_ad_1;
  creative_ad_1.creative_set_id = kCreativeSetIds[0];
  creative_ad_1.total_max = 2;

  CreativeAdInfo creative_ad_2;
  creative_ad_2.creative_set_id = kCreativeSetIds[1];

  AdEventList ad_events;
  const AdEventInfo ad_event = BuildAdEventForTesting(
      creative_ad_2, AdType::kNotificationAd, ConfirmationType::kServed, Now());
  ad_events.push_back(ad_event);
  ad_events.push_back(ad_event);

  const TotalMaxExclusionRule exclusion_rule(ad_events);

  // Act

  // Assert
  EXPECT_TRUE(exclusion_rule.ShouldInclude(creative_ad_1).has_value());
}

TEST_F(HnsAdsTotalMaxExclusionRuleTest, ShouldExcludeIfExceedsZeroCap) {
  // Arrange
  CreativeAdInfo creative_ad;
  creative_ad.creative_set_id = kCreativeSetIds[0];
  creative_ad.total_max = 0;

  const TotalMaxExclusionRule exclusion_rule(/*ad_events*/ {});

  // Act

  // Assert
  EXPECT_FALSE(exclusion_rule.ShouldInclude(creative_ad).has_value());
}

TEST_F(HnsAdsTotalMaxExclusionRuleTest, ShouldExcludeIfExceedsCap) {
  // Arrange
  CreativeAdInfo creative_ad;
  creative_ad.creative_set_id = kCreativeSetIds[0];
  creative_ad.total_max = 2;

  AdEventList ad_events;
  const AdEventInfo ad_event = BuildAdEventForTesting(
      creative_ad, AdType::kNotificationAd, ConfirmationType::kServed, Now());
  ad_events.push_back(ad_event);
  ad_events.push_back(ad_event);

  const TotalMaxExclusionRule exclusion_rule(ad_events);

  // Act

  // Assert
  EXPECT_FALSE(exclusion_rule.ShouldInclude(creative_ad).has_value());
}

}  // namespace hns_ads
