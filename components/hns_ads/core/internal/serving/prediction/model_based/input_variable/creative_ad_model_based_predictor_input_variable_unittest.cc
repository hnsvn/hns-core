/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/serving/prediction/model_based/input_variable/creative_ad_model_based_predictor_input_variable.h"

#include "hns/components/hns_ads/core/internal/common/unittest/unittest_base.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_time_util.h"
#include "hns/components/hns_ads/core/internal/creatives/creative_ad_info.h"
#include "hns/components/hns_ads/core/internal/creatives/creative_ad_unittest_util.h"
#include "hns/components/hns_ads/core/internal/serving/targeting/user_model_builder_unittest_util.h"
#include "hns/components/hns_ads/core/internal/serving/targeting/user_model_info.h"
#include "hns/components/hns_ads/core/internal/user_interaction/ad_events/ad_event_unittest_util.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads {

class HnsAdsCreativeAdPredictorInputVariableTest : public UnitTestBase {};

TEST_F(HnsAdsCreativeAdPredictorInputVariableTest,
       ComputeCreativeAdPredictorInputVariable) {
  // Arrange
  CreativeAdInfo creative_ad =
      BuildCreativeAdForTesting(/*should_use_random_uuids*/ true);
  creative_ad.segment = "parent-child";

  const UserModelInfo user_model = BuildUserModelForTesting(
      /*intent_segments*/ {"parent-child"},
      /*latent_interest_segments*/ {"xyzzy-thud"},
      /*interest_segments*/ {"parent"},
      /*text_embedding_html_events*/ {});

  AdEventList ad_events;
  const AdEventInfo ad_event =
      BuildAdEventForTesting(creative_ad, AdType::kNotificationAd,
                             ConfirmationType::kViewed, Now() - base::Hours(7));
  ad_events.push_back(ad_event);

  // Act
  const CreativeAdPredictorInputVariableInfo input_variable =
      ComputeCreativeAdPredictorInputVariable(creative_ad, user_model,
                                              ad_events);

  // Assert
  CreativeAdPredictorInputVariableInfo expected_input_variable;
  expected_input_variable.intent_segment.does_match_child = true;
  expected_input_variable.intent_segment.does_match_parent = true;
  expected_input_variable.latent_interest_segment.does_match_child = false;
  expected_input_variable.latent_interest_segment.does_match_parent = false;
  expected_input_variable.interest_segment.does_match_child = false;
  expected_input_variable.interest_segment.does_match_parent = true;
  expected_input_variable.last_seen_ad = base::Hours(7);
  expected_input_variable.last_seen_advertiser = base::Hours(7);

  EXPECT_EQ(expected_input_variable, input_variable);
}

}  // namespace hns_ads
