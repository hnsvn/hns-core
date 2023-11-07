/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/fl/predictors/variables/last_notification_ad_was_clicked_predictor_variable.h"

#include <memory>

#include "hns/components/hns_ads/core/internal/common/unittest/unittest_base.h"
#include "hns/components/hns_ads/core/internal/creatives/notification_ads/creative_notification_ad_unittest_util.h"
#include "hns/components/hns_ads/core/internal/creatives/notification_ads/notification_ad_builder.h"
#include "hns/components/hns_ads/core/internal/history/history_manager.h"
#include "hns/components/hns_ads/core/public/ads/notification_ad_info.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads {

class HnsAdsLastNotificationAdWasClickedPredictorVariableTest
    : public UnitTestBase {};

TEST_F(HnsAdsLastNotificationAdWasClickedPredictorVariableTest, GetDataType) {
  std::unique_ptr<PredictorVariableInterface> predictor_variable =
      std::make_unique<LastNotificationAdWasClickedPredictorVariable>();

  // Act

  // Assert
  EXPECT_EQ(hns_federated::mojom::DataType::kBool,
            predictor_variable->GetDataType());
}

TEST_F(HnsAdsLastNotificationAdWasClickedPredictorVariableTest,
       GetValueForNoHistory) {
  // Arrange
  std::unique_ptr<PredictorVariableInterface> predictor_variable =
      std::make_unique<LastNotificationAdWasClickedPredictorVariable>();

  // Act

  // Assert
  EXPECT_EQ("-1", predictor_variable->GetValue());
}

TEST_F(HnsAdsLastNotificationAdWasClickedPredictorVariableTest,
       GetValueAfterExceedingTimeWindow) {
  // Arrange
  std::unique_ptr<PredictorVariableInterface> predictor_variable =
      std::make_unique<LastNotificationAdWasClickedPredictorVariable>();

  const CreativeNotificationAdInfo creative_ad =
      BuildCreativeNotificationAdForTesting(/*should_use_random_uuids*/ true);
  const NotificationAdInfo ad = BuildNotificationAd(creative_ad);

  HistoryManager::GetInstance().Add(ad, ConfirmationType::kViewed);
  HistoryManager::GetInstance().Add(ad, ConfirmationType::kClicked);

  AdvanceClockBy(base::Days(31));

  // Act

  // Assert
  EXPECT_EQ("-1", predictor_variable->GetValue());
}

TEST_F(HnsAdsLastNotificationAdWasClickedPredictorVariableTest,
       GetValueWhenLastClicked) {
  // Arrange
  std::unique_ptr<PredictorVariableInterface> predictor_variable =
      std::make_unique<LastNotificationAdWasClickedPredictorVariable>();

  const CreativeNotificationAdInfo creative_ad =
      BuildCreativeNotificationAdForTesting(/*should_use_random_uuids*/ true);
  const NotificationAdInfo ad = BuildNotificationAd(creative_ad);

  HistoryManager::GetInstance().Add(ad, ConfirmationType::kViewed);
  HistoryManager::GetInstance().Add(ad, ConfirmationType::kClicked);

  // Act

  // Assert
  EXPECT_EQ("1", predictor_variable->GetValue());
}

TEST_F(HnsAdsLastNotificationAdWasClickedPredictorVariableTest,
       GetValueWhenLastViewed) {
  // Arrange
  std::unique_ptr<PredictorVariableInterface> predictor_variable =
      std::make_unique<LastNotificationAdWasClickedPredictorVariable>();

  const CreativeNotificationAdInfo creative_ad =
      BuildCreativeNotificationAdForTesting(/*should_use_random_uuids*/ true);
  const NotificationAdInfo ad = BuildNotificationAd(creative_ad);

  HistoryManager::GetInstance().Add(ad, ConfirmationType::kClicked);
  HistoryManager::GetInstance().Add(ad, ConfirmationType::kViewed);

  // Act

  // Assert
  EXPECT_EQ("0", predictor_variable->GetValue());
}

}  // namespace hns_ads
