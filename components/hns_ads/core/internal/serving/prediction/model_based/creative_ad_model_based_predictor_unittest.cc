/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/serving/prediction/model_based/creative_ad_model_based_predictor.h"

#include "base/test/scoped_feature_list.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_base.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_time_util.h"
#include "hns/components/hns_ads/core/internal/creatives/notification_ads/creative_notification_ad_info.h"
#include "hns/components/hns_ads/core/internal/creatives/notification_ads/creative_notification_ad_unittest_util.h"
#include "hns/components/hns_ads/core/internal/serving/prediction/model_based/creative_ad_model_based_predictor_feature.h"
#include "hns/components/hns_ads/core/internal/serving/targeting/user_model_builder_unittest_util.h"
#include "hns/components/hns_ads/core/internal/serving/targeting/user_model_info.h"
#include "hns/components/hns_ads/core/internal/user_interaction/ad_events/ad_event_unittest_util.h"
#include "third_party/abseil-cpp/absl/types/optional.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads {

class HnsAdsCreativeAdModelBasedPredictorTest : public UnitTestBase {};

TEST_F(HnsAdsCreativeAdModelBasedPredictorTest, PredictCreativeAd) {
  // Arrange
  const UserModelInfo user_model = BuildUserModelForTesting(
      /*intent_segments*/ {"parent-child"},
      /*latent_interest_segments*/ {"parent-child"},
      /*interest_segments*/ {"parent-child"},
      /*text_embedding_html_events*/ {});

  CreativeNotificationAdList creative_ads;
  const CreativeNotificationAdInfo creative_ad =
      BuildCreativeNotificationAdForTesting(/*should_use_random_uuids*/
                                            true);
  creative_ads.push_back(creative_ad);

  AdEventList ad_events;
  const AdEventInfo ad_event = BuildAdEventForTesting(
      creative_ad, AdType::kNotificationAd, ConfirmationType::kViewed, Now());
  ad_events.push_back(ad_event);

  // Act
  const absl::optional<CreativeNotificationAdInfo> predicted_creative_ad =
      MaybePredictCreativeAd(creative_ads, user_model, ad_events);

  // Assert
  EXPECT_TRUE(predicted_creative_ad);
}

TEST_F(HnsAdsCreativeAdModelBasedPredictorTest, DoNotPredictCreativeAd) {
  // Arrange
  std::vector<base::test::FeatureRefAndParams> enabled_features;
  base::FieldTrialParams params;
  params["child_intent_segment_ad_predictor_weight"] = "0.0";
  params["parent_intent_segment_ad_predictor_weight"] = "0.0";
  params["child_latent_interest_segment_ad_predictor_weight"] = "0.0";
  params["parent_latent_interest_segment_ad_predictor_weight"] = "0.0";
  params["child_interest_segment_ad_predictor_weight"] = "0.0";
  params["parent_interest_segment_ad_predictor_weight"] = "0.0";
  params["last_seen_ad_predictor_weight"] = "0.0";
  params["last_seen_advertiser_ad_predictor_weight"] = "0.0";
  params["priority_ad_predictor_weight"] = "0.0";
  enabled_features.emplace_back(CreativeAdModelBasedPredictorFeature, params);

  const std::vector<base::test::FeatureRef> disabled_features;

  base::test::ScopedFeatureList scoped_feature_list;
  scoped_feature_list.InitWithFeaturesAndParameters(enabled_features,
                                                    disabled_features);

  const UserModelInfo user_model = BuildUserModelForTesting(
      /*intent_segments*/ {"parent-child"},
      /*latent_interest_segments*/ {"parent-child"},
      /*interest_segments*/ {"parent-child"},
      /*text_embedding_html_events*/ {});

  CreativeNotificationAdList creative_ads;
  const CreativeNotificationAdInfo creative_ad =
      BuildCreativeNotificationAdForTesting(/*should_use_random_uuids*/
                                            true);
  creative_ads.push_back(creative_ad);

  AdEventList ad_events;
  const AdEventInfo ad_event = BuildAdEventForTesting(
      creative_ad, AdType::kNotificationAd, ConfirmationType::kViewed, Now());
  ad_events.push_back(ad_event);

  // Act
  const absl::optional<CreativeNotificationAdInfo> predicted_creative_ad =
      MaybePredictCreativeAd(creative_ads, user_model, ad_events);

  // Assert
  EXPECT_FALSE(predicted_creative_ad);
}

}  // namespace hns_ads
