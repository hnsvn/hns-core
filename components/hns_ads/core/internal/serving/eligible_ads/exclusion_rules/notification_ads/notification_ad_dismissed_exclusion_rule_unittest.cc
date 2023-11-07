/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/serving/eligible_ads/exclusion_rules/notification_ads/notification_ad_dismissed_exclusion_rule.h"

#include <vector>

#include "base/test/scoped_feature_list.h"
#include "hns/components/hns_ads/core/internal/ads/ad_unittest_constants.h"
#include "hns/components/hns_ads/core/internal/ads/ad_unittest_util.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_base.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_time_util.h"
#include "hns/components/hns_ads/core/internal/creatives/creative_ad_info.h"
#include "hns/components/hns_ads/core/internal/serving/eligible_ads/exclusion_rules/exclusion_rule_feature.h"
#include "hns/components/hns_ads/core/internal/user_interaction/ad_events/ad_event_builder.h"
#include "hns/components/hns_ads/core/internal/user_interaction/ad_events/ad_event_unittest_util.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads {

namespace {

constexpr const char* kCampaignIds[] = {"60267cee-d5bb-4a0d-baaf-91cd7f18e07e",
                                        "90762cee-d5bb-4a0d-baaf-61cd7f18e07e"};

}  // namespace

class HnsAdsDismissedExclusionRuleTest : public UnitTestBase {};

TEST_F(HnsAdsDismissedExclusionRuleTest, ShouldIncludeIfThereAreNoAdEvents) {
  // Arrange
  CreativeAdInfo creative_ad;
  creative_ad.creative_instance_id = kCreativeInstanceId;
  creative_ad.campaign_id = kCampaignIds[0];

  const NotificationAdDismissedExclusionRule exclusion_rule(/*ad_events*/ {});

  // Act

  // Assert
  EXPECT_TRUE(exclusion_rule.ShouldInclude(creative_ad).has_value());
}

TEST_F(HnsAdsDismissedExclusionRuleTest,
       ShouldIncludeWithSameCampaignIdWithin2DaysIfDismissedOnce) {
  // Arrange
  base::FieldTrialParams params;
  params["should_exclude_ad_if_dismissed_within_time_window"] = "2d";
  std::vector<base::test::FeatureRefAndParams> enabled_features;
  enabled_features.emplace_back(kExclusionRulesFeature, params);

  const std::vector<base::test::FeatureRef> disabled_features;

  base::test::ScopedFeatureList scoped_feature_list;
  scoped_feature_list.InitWithFeaturesAndParameters(enabled_features,
                                                    disabled_features);

  CreativeAdInfo creative_ad;
  creative_ad.creative_instance_id = kCreativeInstanceId;
  creative_ad.campaign_id = kCampaignIds[0];

  const std::vector<ConfirmationType> confirmation_types = {
      ConfirmationType::kViewed,
      ConfirmationType::kDismissed,
  };

  AdEventList ad_events;
  for (const auto& confirmation_type : confirmation_types) {
    const AdEventInfo ad_event = BuildAdEventForTesting(
        creative_ad, AdType::kNotificationAd, confirmation_type, Now());
    ad_events.push_back(ad_event);
    AdvanceClockBy(base::Minutes(5));
  }

  const NotificationAdDismissedExclusionRule exclusion_rule(ad_events);

  // Act

  // Assert
  EXPECT_TRUE(exclusion_rule.ShouldInclude(creative_ad).has_value());
}

TEST_F(
    HnsAdsDismissedExclusionRuleTest,
    ShouldIncludeWithSameCampaignIdWithin2DaysIfDismissedOnceForMultipleTypes) {
  // Arrange
  base::FieldTrialParams params;
  params["should_exclude_ad_if_dismissed_within_time_window"] = "2d";
  std::vector<base::test::FeatureRefAndParams> enabled_features;
  enabled_features.emplace_back(kExclusionRulesFeature, params);

  const std::vector<base::test::FeatureRef> disabled_features;

  base::test::ScopedFeatureList scoped_feature_list;
  scoped_feature_list.InitWithFeaturesAndParameters(enabled_features,
                                                    disabled_features);

  CreativeAdInfo creative_ad;
  creative_ad.creative_instance_id = kCreativeInstanceId;
  creative_ad.campaign_id = kCampaignIds[0];

  AdEventList ad_events;

  const AdInfo ad_1 = BuildAdForTesting(AdType::kNotificationAd,
                                        /*should_use_random_uuids*/ true);
  const AdEventInfo ad_event_1 =
      BuildAdEvent(ad_1, ConfirmationType::kDismissed, Now());
  ad_events.push_back(ad_event_1);

  const AdInfo ad_2 = BuildAdForTesting(AdType::kNewTabPageAd,
                                        /*should_use_random_uuids*/ true);
  const AdEventInfo ad_event_2 =
      BuildAdEvent(ad_2, ConfirmationType::kDismissed, Now());
  ad_events.push_back(ad_event_2);

  const AdInfo ad_3 = BuildAdForTesting(AdType::kPromotedContentAd,
                                        /*should_use_random_uuids*/ true);
  const AdEventInfo ad_event_3 =
      BuildAdEvent(ad_3, ConfirmationType::kDismissed, Now());
  ad_events.push_back(ad_event_3);

  const AdInfo ad_4 = BuildAdForTesting(AdType::kSearchResultAd,
                                        /*should_use_random_uuids*/ true);
  const AdEventInfo ad_event_4 =
      BuildAdEvent(ad_4, ConfirmationType::kDismissed, Now());
  ad_events.push_back(ad_event_4);

  const NotificationAdDismissedExclusionRule exclusion_rule(ad_events);

  // Act

  // Assert
  EXPECT_TRUE(exclusion_rule.ShouldInclude(creative_ad).has_value());
}

TEST_F(HnsAdsDismissedExclusionRuleTest,
       ShouldIncludeWithSameCampaignIdWithin2DaysIfDismissedThenClicked) {
  // Arrange
  base::FieldTrialParams params;
  params["should_exclude_ad_if_dismissed_within_time_window"] = "2d";
  std::vector<base::test::FeatureRefAndParams> enabled_features;
  enabled_features.emplace_back(kExclusionRulesFeature, params);

  const std::vector<base::test::FeatureRef> disabled_features;

  base::test::ScopedFeatureList scoped_feature_list;
  scoped_feature_list.InitWithFeaturesAndParameters(enabled_features,
                                                    disabled_features);

  CreativeAdInfo creative_ad;
  creative_ad.creative_instance_id = kCreativeInstanceId;
  creative_ad.campaign_id = kCampaignIds[0];

  const std::vector<ConfirmationType> confirmation_types = {
      ConfirmationType::kViewed, ConfirmationType::kDismissed,
      ConfirmationType::kViewed, ConfirmationType::kClicked};

  AdEventList ad_events;
  for (const auto& confirmation_type : confirmation_types) {
    const AdEventInfo ad_event = BuildAdEventForTesting(
        creative_ad, AdType::kNotificationAd, confirmation_type, Now());
    ad_events.push_back(ad_event);
    AdvanceClockBy(base::Minutes(5));
  }

  const NotificationAdDismissedExclusionRule exclusion_rule(ad_events);

  // Act

  // Assert
  EXPECT_TRUE(exclusion_rule.ShouldInclude(creative_ad).has_value());
}

TEST_F(HnsAdsDismissedExclusionRuleTest,
       ShouldIncludeWithSameCampaignIdAfter2DaysIfDismissedThenClicked) {
  // Arrange
  base::FieldTrialParams params;
  params["should_exclude_ad_if_dismissed_within_time_window"] = "2d";
  std::vector<base::test::FeatureRefAndParams> enabled_features;
  enabled_features.emplace_back(kExclusionRulesFeature, params);

  const std::vector<base::test::FeatureRef> disabled_features;

  base::test::ScopedFeatureList scoped_feature_list;
  scoped_feature_list.InitWithFeaturesAndParameters(enabled_features,
                                                    disabled_features);

  CreativeAdInfo creative_ad;
  creative_ad.creative_instance_id = kCreativeInstanceId;
  creative_ad.campaign_id = kCampaignIds[0];

  const std::vector<ConfirmationType> confirmation_types = {
      ConfirmationType::kViewed, ConfirmationType::kDismissed,
      ConfirmationType::kViewed, ConfirmationType::kClicked};

  AdEventList ad_events;
  for (const auto& confirmation_type : confirmation_types) {
    const AdEventInfo ad_event = BuildAdEventForTesting(
        creative_ad, AdType::kNotificationAd, confirmation_type, Now());
    ad_events.push_back(ad_event);
    AdvanceClockBy(base::Minutes(5));
  }

  const NotificationAdDismissedExclusionRule exclusion_rule(ad_events);

  AdvanceClockBy(base::Days(2) -
                 (base::Minutes(5) * confirmation_types.size()));

  // Act

  // Assert
  EXPECT_TRUE(exclusion_rule.ShouldInclude(creative_ad).has_value());
}

TEST_F(HnsAdsDismissedExclusionRuleTest,
       ShouldIncludeWithSameCampaignIdWithin2DaysIfClickedThenDismissed) {
  // Arrange
  base::FieldTrialParams params;
  params["should_exclude_ad_if_dismissed_within_time_window"] = "2d";
  std::vector<base::test::FeatureRefAndParams> enabled_features;
  enabled_features.emplace_back(kExclusionRulesFeature, params);

  const std::vector<base::test::FeatureRef> disabled_features;

  base::test::ScopedFeatureList scoped_feature_list;
  scoped_feature_list.InitWithFeaturesAndParameters(enabled_features,
                                                    disabled_features);

  CreativeAdInfo creative_ad;
  creative_ad.creative_instance_id = kCreativeInstanceId;
  creative_ad.campaign_id = kCampaignIds[0];

  const std::vector<ConfirmationType> confirmation_types = {
      ConfirmationType::kViewed, ConfirmationType::kClicked,
      ConfirmationType::kViewed, ConfirmationType::kDismissed};

  AdEventList ad_events;
  for (const auto& confirmation_type : confirmation_types) {
    const AdEventInfo ad_event = BuildAdEventForTesting(
        creative_ad, AdType::kNotificationAd, confirmation_type, Now());
    ad_events.push_back(ad_event);
    AdvanceClockBy(base::Minutes(5));
  }

  const NotificationAdDismissedExclusionRule exclusion_rule(ad_events);

  // Act

  // Assert
  EXPECT_TRUE(exclusion_rule.ShouldInclude(creative_ad).has_value());
}

TEST_F(HnsAdsDismissedExclusionRuleTest,
       ShouldIncludeWithSameCampaignIdAfter2DaysIfClickedThenDismissed) {
  // Arrange
  base::FieldTrialParams params;
  params["should_exclude_ad_if_dismissed_within_time_window"] = "2d";
  std::vector<base::test::FeatureRefAndParams> enabled_features;
  enabled_features.emplace_back(kExclusionRulesFeature, params);

  const std::vector<base::test::FeatureRef> disabled_features;

  base::test::ScopedFeatureList scoped_feature_list;
  scoped_feature_list.InitWithFeaturesAndParameters(enabled_features,
                                                    disabled_features);

  CreativeAdInfo creative_ad;
  creative_ad.creative_instance_id = kCreativeInstanceId;
  creative_ad.campaign_id = kCampaignIds[0];

  const std::vector<ConfirmationType> confirmation_types = {
      ConfirmationType::kViewed, ConfirmationType::kClicked,
      ConfirmationType::kViewed, ConfirmationType::kDismissed};

  AdEventList ad_events;
  for (const auto& confirmation_type : confirmation_types) {
    const AdEventInfo ad_event = BuildAdEventForTesting(
        creative_ad, AdType::kNotificationAd, confirmation_type, Now());
    ad_events.push_back(ad_event);
    AdvanceClockBy(base::Minutes(5));
  }

  const NotificationAdDismissedExclusionRule exclusion_rule(ad_events);

  AdvanceClockBy(base::Days(2) -
                 (base::Minutes(5) * confirmation_types.size()));

  // Act

  // Assert
  EXPECT_TRUE(exclusion_rule.ShouldInclude(creative_ad).has_value());
}

TEST_F(HnsAdsDismissedExclusionRuleTest,
       ShouldIncludeWithSameCampaignIdAfter2DaysIfClickedThenDismissedTwice) {
  // Arrange
  base::FieldTrialParams params;
  params["should_exclude_ad_if_dismissed_within_time_window"] = "2d";
  std::vector<base::test::FeatureRefAndParams> enabled_features;
  enabled_features.emplace_back(kExclusionRulesFeature, params);

  const std::vector<base::test::FeatureRef> disabled_features;

  base::test::ScopedFeatureList scoped_feature_list;
  scoped_feature_list.InitWithFeaturesAndParameters(enabled_features,
                                                    disabled_features);

  CreativeAdInfo creative_ad;
  creative_ad.creative_instance_id = kCreativeInstanceId;
  creative_ad.campaign_id = kCampaignIds[0];

  const std::vector<ConfirmationType> confirmation_types = {
      ConfirmationType::kViewed, ConfirmationType::kClicked,
      ConfirmationType::kViewed, ConfirmationType::kDismissed,
      ConfirmationType::kViewed, ConfirmationType::kDismissed};

  AdEventList ad_events;
  for (const auto& confirmation_type : confirmation_types) {
    const AdEventInfo ad_event = BuildAdEventForTesting(
        creative_ad, AdType::kNotificationAd, confirmation_type, Now());
    ad_events.push_back(ad_event);
    AdvanceClockBy(base::Minutes(5));
  }

  const NotificationAdDismissedExclusionRule exclusion_rule(ad_events);

  AdvanceClockBy(base::Days(2));

  // Act

  // Assert
  EXPECT_TRUE(exclusion_rule.ShouldInclude(creative_ad).has_value());
}

TEST_F(HnsAdsDismissedExclusionRuleTest,
       ShouldExcludeWithSameCampaignIdWithin2DaysIfClickedThenDismissedTwice) {
  // Arrange
  base::FieldTrialParams params;
  params["should_exclude_ad_if_dismissed_within_time_window"] = "2d";
  std::vector<base::test::FeatureRefAndParams> enabled_features;
  enabled_features.emplace_back(kExclusionRulesFeature, params);

  const std::vector<base::test::FeatureRef> disabled_features;

  base::test::ScopedFeatureList scoped_feature_list;
  scoped_feature_list.InitWithFeaturesAndParameters(enabled_features,
                                                    disabled_features);

  CreativeAdInfo creative_ad;
  creative_ad.creative_instance_id = kCreativeInstanceId;
  creative_ad.campaign_id = kCampaignIds[0];

  const std::vector<ConfirmationType> confirmation_types = {
      ConfirmationType::kViewed, ConfirmationType::kClicked,
      ConfirmationType::kViewed, ConfirmationType::kDismissed,
      ConfirmationType::kViewed, ConfirmationType::kDismissed};

  AdEventList ad_events;
  for (const auto& confirmation_type : confirmation_types) {
    const AdEventInfo ad_event = BuildAdEventForTesting(
        creative_ad, AdType::kNotificationAd, confirmation_type, Now());
    ad_events.push_back(ad_event);
    AdvanceClockBy(base::Minutes(5));
  }

  const NotificationAdDismissedExclusionRule exclusion_rule(ad_events);

  // Act

  // Assert
  EXPECT_FALSE(exclusion_rule.ShouldInclude(creative_ad).has_value());
}

TEST_F(HnsAdsDismissedExclusionRuleTest,
       ShouldIncludeWithSameCampaignIdIfClickedThenDismissedTwiceWhenDisabled) {
  // Arrange
  base::FieldTrialParams params;
  params["should_exclude_ad_if_dismissed_within_time_window"] = "0s";
  std::vector<base::test::FeatureRefAndParams> enabled_features;
  enabled_features.emplace_back(kExclusionRulesFeature, params);

  const std::vector<base::test::FeatureRef> disabled_features;

  base::test::ScopedFeatureList scoped_feature_list;
  scoped_feature_list.InitWithFeaturesAndParameters(enabled_features,
                                                    disabled_features);

  CreativeAdInfo creative_ad;
  creative_ad.creative_instance_id = kCreativeInstanceId;
  creative_ad.campaign_id = kCampaignIds[0];

  const std::vector<ConfirmationType> confirmation_types = {
      ConfirmationType::kViewed, ConfirmationType::kClicked,
      ConfirmationType::kViewed, ConfirmationType::kDismissed,
      ConfirmationType::kViewed, ConfirmationType::kDismissed};

  AdEventList ad_events;
  for (const auto& confirmation_type : confirmation_types) {
    const AdEventInfo ad_event = BuildAdEventForTesting(
        creative_ad, AdType::kNotificationAd, confirmation_type, Now());
    ad_events.push_back(ad_event);
  }

  const NotificationAdDismissedExclusionRule exclusion_rule(ad_events);

  // Act

  // Assert
  EXPECT_TRUE(exclusion_rule.ShouldInclude(creative_ad).has_value());
}

TEST_F(HnsAdsDismissedExclusionRuleTest,
       ShouldIncludeWithDifferentCampaignIdWithin2Days) {
  // Arrange
  base::FieldTrialParams params;
  params["should_exclude_ad_if_dismissed_within_time_window"] = "2d";
  std::vector<base::test::FeatureRefAndParams> enabled_features;
  enabled_features.emplace_back(kExclusionRulesFeature, params);

  const std::vector<base::test::FeatureRef> disabled_features;

  base::test::ScopedFeatureList scoped_feature_list;
  scoped_feature_list.InitWithFeaturesAndParameters(enabled_features,
                                                    disabled_features);

  CreativeAdInfo creative_ad_1;
  creative_ad_1.creative_instance_id = kCreativeInstanceId;
  creative_ad_1.campaign_id = kCampaignIds[0];

  CreativeAdInfo creative_ad_2;
  creative_ad_2.creative_instance_id = kCreativeInstanceId;
  creative_ad_2.campaign_id = kCampaignIds[1];

  const std::vector<ConfirmationType> confirmation_types = {
      ConfirmationType::kViewed, ConfirmationType::kDismissed,
      ConfirmationType::kViewed, ConfirmationType::kDismissed};

  AdEventList ad_events;
  for (const auto& confirmation_type : confirmation_types) {
    const AdEventInfo ad_event = BuildAdEventForTesting(
        creative_ad_2, AdType::kNotificationAd, confirmation_type, Now());
    ad_events.push_back(ad_event);
    AdvanceClockBy(base::Minutes(5));
  }

  const NotificationAdDismissedExclusionRule exclusion_rule(ad_events);

  // Act

  // Assert
  EXPECT_TRUE(exclusion_rule.ShouldInclude(creative_ad_1).has_value());
}

TEST_F(HnsAdsDismissedExclusionRuleTest,
       ShouldIncludeWithDifferentCampaignIdAfter2Days) {
  // Arrange
  CreativeAdInfo creative_ad_1;
  creative_ad_1.creative_instance_id = kCreativeInstanceId;
  creative_ad_1.campaign_id = kCampaignIds[0];

  CreativeAdInfo creative_ad_2;
  creative_ad_2.creative_instance_id = kCreativeInstanceId;
  creative_ad_2.campaign_id = kCampaignIds[1];

  const std::vector<ConfirmationType> confirmation_types = {
      ConfirmationType::kViewed, ConfirmationType::kDismissed,
      ConfirmationType::kViewed, ConfirmationType::kDismissed};

  AdEventList ad_events;
  for (const auto& confirmation_type : confirmation_types) {
    const AdEventInfo ad_event = BuildAdEventForTesting(
        creative_ad_2, AdType::kNotificationAd, confirmation_type, Now());
    ad_events.push_back(ad_event);
    AdvanceClockBy(base::Minutes(5));
  }

  const NotificationAdDismissedExclusionRule exclusion_rule(ad_events);

  AdvanceClockBy(base::Days(2) -
                 (base::Minutes(5) * confirmation_types.size()));

  // Act

  // Assert
  EXPECT_TRUE(exclusion_rule.ShouldInclude(creative_ad_1).has_value());
}

}  // namespace hns_ads
