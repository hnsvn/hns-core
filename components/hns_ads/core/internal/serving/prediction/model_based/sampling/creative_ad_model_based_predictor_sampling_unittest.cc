/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/serving/prediction/model_based/sampling/creative_ad_model_based_predictor_sampling.h"

#include "hns/components/hns_ads/core/internal/common/unittest/unittest_base.h"
#include "hns/components/hns_ads/core/internal/creatives/notification_ads/creative_notification_ad_info.h"
#include "hns/components/hns_ads/core/internal/creatives/notification_ads/creative_notification_ad_unittest_util.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads {

class HnsAdsCreativeAdPredictorSamplingTest : public UnitTestBase {};

TEST_F(HnsAdsCreativeAdPredictorSamplingTest, SampleCreativeAd) {
  // Arrange
  CreativeAdPredictorList<CreativeNotificationAdInfo> creative_ad_predictors;

  CreativeAdPredictorInfo<CreativeNotificationAdInfo> creative_ad_predictor;
  creative_ad_predictor.creative_ad =
      BuildCreativeNotificationAdForTesting(/*should_use_random_uuids*/ true);
  creative_ad_predictor.score = 1.0;
  creative_ad_predictors.push_back(creative_ad_predictor);

  // Act

  // Assert
  EXPECT_EQ(creative_ad_predictor.creative_ad,
            MaybeSampleCreativeAd(creative_ad_predictors));
}

TEST_F(HnsAdsCreativeAdPredictorSamplingTest,
       DeterministicallySampleCreativeAdWhenOneNonZeroScore) {
  // Arrange
  CreativeAdPredictorList<CreativeNotificationAdInfo> creative_ad_predictors;

  CreativeAdPredictorInfo<CreativeNotificationAdInfo> creative_ad_predictor_1;
  creative_ad_predictor_1.creative_ad =
      BuildCreativeNotificationAdForTesting(/*should_use_random_uuids*/ true);
  creative_ad_predictor_1.score = 0.0;
  creative_ad_predictors.push_back(creative_ad_predictor_1);

  CreativeAdPredictorInfo<CreativeNotificationAdInfo> creative_ad_predictor_2;
  creative_ad_predictor_2.creative_ad =
      BuildCreativeNotificationAdForTesting(/*should_use_random_uuids*/ true);
  creative_ad_predictor_2.score = 1.0;
  creative_ad_predictors.push_back(creative_ad_predictor_2);

  CreativeAdPredictorInfo<CreativeNotificationAdInfo> creative_ad_predictor_3;
  creative_ad_predictor_3.creative_ad =
      BuildCreativeNotificationAdForTesting(/*should_use_random_uuids*/ true);
  creative_ad_predictor_3.score = 0.0;
  creative_ad_predictors.push_back(creative_ad_predictor_3);

  // Act

  // Assert
  for (int i = 0; i < 10; i++) {
    EXPECT_EQ(creative_ad_predictor_2.creative_ad,
              MaybeSampleCreativeAd(creative_ad_predictors));
  }
}

TEST_F(HnsAdsCreativeAdPredictorSamplingTest,
       ProbabilisticallySampleCreativeAd) {
  // Arrange
  CreativeAdPredictorList<CreativeNotificationAdInfo> creative_ad_predictors;

  CreativeAdPredictorInfo<CreativeNotificationAdInfo> creative_ad_predictor_1;
  creative_ad_predictor_1.creative_ad =
      BuildCreativeNotificationAdForTesting(/*should_use_random_uuids*/ true);
  creative_ad_predictor_1.score = 3.0;
  creative_ad_predictors.push_back(creative_ad_predictor_1);

  CreativeAdPredictorInfo<CreativeNotificationAdInfo> creative_ad_predictor_2;
  creative_ad_predictor_2.creative_ad =
      BuildCreativeNotificationAdForTesting(/*should_use_random_uuids*/ true);
  creative_ad_predictor_2.score = 3.0;
  creative_ad_predictors.push_back(creative_ad_predictor_2);

  // Act
  int creative_ad_1_count = 0;
  int creative_ad_2_count = 0;

  // P(X>1) > 0.99999999 with X~Bin(n=25, p=0.5), i.e. less than 1 in 100M tests
  // are expected to fail.
  for (int i = 0; i < 25; i++) {
    const absl::optional<CreativeNotificationAdInfo> creative_ad =
        MaybeSampleCreativeAd(creative_ad_predictors);
    ASSERT_TRUE(creative_ad);

    if (creative_ad->creative_instance_id ==
        creative_ad_predictor_1.creative_ad.creative_instance_id) {
      creative_ad_1_count++;
    } else if (creative_ad->creative_instance_id ==
               creative_ad_predictor_2.creative_ad.creative_instance_id) {
      creative_ad_2_count++;
    }
  }

  // Assert
  EXPECT_NE(0, creative_ad_1_count);
  EXPECT_NE(0, creative_ad_2_count);
}

TEST_F(HnsAdsCreativeAdPredictorSamplingTest, DoNotSampleCreativeAd) {
  // Arrange
  CreativeAdPredictorList<CreativeNotificationAdInfo> creative_ad_predictors;

  CreativeAdPredictorInfo<CreativeNotificationAdInfo> creative_ad_predictor;
  creative_ad_predictor.creative_ad =
      BuildCreativeNotificationAdForTesting(/*should_use_random_uuids*/ true);
  creative_ad_predictor.score = 0.0;
  creative_ad_predictors.push_back(creative_ad_predictor);

  // Act

  // Assert
  EXPECT_FALSE(MaybeSampleCreativeAd(creative_ad_predictors));
}

}  // namespace hns_ads
