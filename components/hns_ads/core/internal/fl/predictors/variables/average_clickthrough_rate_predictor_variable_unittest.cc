/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/fl/predictors/variables/average_clickthrough_rate_predictor_variable.h"

#include <memory>

#include "hns/components/hns_ads/core/internal/common/unittest/unittest_base.h"
#include "hns/components/hns_ads/core/internal/creatives/notification_ads/creative_notification_ad_unittest_util.h"
#include "hns/components/hns_ads/core/internal/creatives/notification_ads/notification_ad_builder.h"
#include "hns/components/hns_ads/core/internal/history/history_manager.h"
#include "hns/components/hns_ads/core/public/ads/notification_ad_info.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads {

class HnsAdsAverageClickthroughRatePredictorVariableTest
    : public UnitTestBase {};

TEST_F(HnsAdsAverageClickthroughRatePredictorVariableTest, GetDataType) {
  // Arrange
  std::unique_ptr<PredictorVariableInterface> predictor_variable =
      std::make_unique<AverageClickthroughRatePredictorVariable>(
          /*time_window*/ base::Days(7));

  // Act

  // Assert
  EXPECT_EQ(hns_federated::mojom::DataType::kDouble,
            predictor_variable->GetDataType());
}

TEST_F(HnsAdsAverageClickthroughRatePredictorVariableTest,
       GetValueForNoHistory) {
  // Arrange
  std::unique_ptr<PredictorVariableInterface> predictor_variable =
      std::make_unique<AverageClickthroughRatePredictorVariable>(
          /*time_window*/ base::Days(1));

  // Act

  // Assert
  EXPECT_EQ("-1", predictor_variable->GetValue());
}

TEST_F(HnsAdsAverageClickthroughRatePredictorVariableTest,
       GetValueAfterExceedingTimeWindow) {
  // Arrange
  std::unique_ptr<PredictorVariableInterface> predictor_variable =
      std::make_unique<AverageClickthroughRatePredictorVariable>(
          /*time_window*/ base::Days(1));

  const CreativeNotificationAdInfo creative_ad =
      BuildCreativeNotificationAdForTesting(/*should_use_random_uuids*/ true);
  const NotificationAdInfo ad = BuildNotificationAd(creative_ad);

  HistoryManager::GetInstance().Add(ad, ConfirmationType::kViewed);
  HistoryManager::GetInstance().Add(ad, ConfirmationType::kViewed);
  HistoryManager::GetInstance().Add(ad, ConfirmationType::kClicked);

  AdvanceClockBy(base::Days(2));

  // Act

  // Assert
  EXPECT_EQ("-1", predictor_variable->GetValue());
}

TEST_F(HnsAdsAverageClickthroughRatePredictorVariableTest,
       GetValueForNoClicks) {
  // Arrange
  std::unique_ptr<PredictorVariableInterface> predictor_variable =
      std::make_unique<AverageClickthroughRatePredictorVariable>(
          /*time_window*/ base::Days(1));

  const CreativeNotificationAdInfo creative_ad =
      BuildCreativeNotificationAdForTesting(/*should_use_random_uuids*/ true);
  const NotificationAdInfo ad = BuildNotificationAd(creative_ad);

  HistoryManager::GetInstance().Add(ad, ConfirmationType::kViewed);

  // Act

  // Assert
  EXPECT_EQ("0", predictor_variable->GetValue());
}

TEST_F(HnsAdsAverageClickthroughRatePredictorVariableTest,
       GetValueForOneClick) {
  // Arrange
  std::unique_ptr<PredictorVariableInterface> predictor_variable =
      std::make_unique<AverageClickthroughRatePredictorVariable>(
          /*time_window*/ base::Days(1));

  const CreativeNotificationAdInfo creative_ad =
      BuildCreativeNotificationAdForTesting(/*should_use_random_uuids*/ true);
  const NotificationAdInfo ad = BuildNotificationAd(creative_ad);

  HistoryManager::GetInstance().Add(ad, ConfirmationType::kViewed);
  HistoryManager::GetInstance().Add(ad, ConfirmationType::kClicked);

  // Act

  // Assert
  EXPECT_EQ("1", predictor_variable->GetValue());
}

TEST_F(HnsAdsAverageClickthroughRatePredictorVariableTest,
       GetValueForMoreThanOneClick) {
  // Arrange
  std::unique_ptr<PredictorVariableInterface> predictor_variable =
      std::make_unique<AverageClickthroughRatePredictorVariable>(
          /*time_window*/ base::Days(1));

  const CreativeNotificationAdInfo creative_ad =
      BuildCreativeNotificationAdForTesting(/*should_use_random_uuids*/ true);
  const NotificationAdInfo ad = BuildNotificationAd(creative_ad);

  HistoryManager::GetInstance().Add(ad, ConfirmationType::kViewed);
  HistoryManager::GetInstance().Add(ad, ConfirmationType::kClicked);
  HistoryManager::GetInstance().Add(ad, ConfirmationType::kClicked);

  // Act

  // Assert
  EXPECT_EQ("-1", predictor_variable->GetValue());
}

TEST_F(HnsAdsAverageClickthroughRatePredictorVariableTest, GetValue) {
  // Arrange
  std::unique_ptr<PredictorVariableInterface> predictor_variable =
      std::make_unique<AverageClickthroughRatePredictorVariable>(
          /*time_window*/ base::Days(1));

  const CreativeNotificationAdInfo creative_ad =
      BuildCreativeNotificationAdForTesting(/*should_use_random_uuids*/ true);
  const NotificationAdInfo ad = BuildNotificationAd(creative_ad);

  HistoryManager::GetInstance().Add(ad, ConfirmationType::kViewed);
  HistoryManager::GetInstance().Add(ad, ConfirmationType::kViewed);
  HistoryManager::GetInstance().Add(ad, ConfirmationType::kViewed);
  HistoryManager::GetInstance().Add(ad, ConfirmationType::kClicked);

  // Act

  // Assert
  EXPECT_EQ("0.3333333333333333", predictor_variable->GetValue());
}

}  // namespace hns_ads
