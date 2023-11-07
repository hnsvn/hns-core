/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/fl/predictors/variables/time_since_last_user_activity_event_predictor_variable.h"

#include <memory>

#include "hns/components/hns_ads/core/internal/common/unittest/unittest_base.h"
#include "hns/components/hns_ads/core/internal/user_attention/user_activity/user_activity_manager.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads {

class HnsAdsTimeSinceLastUserActivityEventPredictorVariableTest
    : public UnitTestBase {};

TEST_F(HnsAdsTimeSinceLastUserActivityEventPredictorVariableTest,
       GetDataType) {
  // Arrange
  std::unique_ptr<PredictorVariableInterface> predictor_variable =
      std::make_unique<TimeSinceLastUserActivityEventPredictorVariable>(
          UserActivityEventType::kOpenedNewTab,
          hns_federated::mojom::CovariateType::
              kTimeSinceLastOpenedNewTabEvent);

  // Act

  // Assert
  EXPECT_EQ(hns_federated::mojom::DataType::kInt,
            predictor_variable->GetDataType());
}

TEST_F(HnsAdsTimeSinceLastUserActivityEventPredictorVariableTest,
       GetValueForNoHistory) {
  // Arrange
  std::unique_ptr<PredictorVariableInterface> predictor_variable =
      std::make_unique<TimeSinceLastUserActivityEventPredictorVariable>(
          UserActivityEventType::kOpenedNewTab,
          hns_federated::mojom::CovariateType::
              kTimeSinceLastOpenedNewTabEvent);

  // Act

  // Assert
  EXPECT_EQ("-1", predictor_variable->GetValue());
}

TEST_F(HnsAdsTimeSinceLastUserActivityEventPredictorVariableTest, GetValue) {
  // Arrange
  std::unique_ptr<PredictorVariableInterface> predictor_variable =
      std::make_unique<TimeSinceLastUserActivityEventPredictorVariable>(
          UserActivityEventType::kOpenedNewTab,
          hns_federated::mojom::CovariateType::
              kTimeSinceLastOpenedNewTabEvent);

  UserActivityManager::GetInstance().RecordEvent(
      UserActivityEventType::kOpenedNewTab);

  AdvanceClockBy(base::Minutes(2));

  UserActivityManager::GetInstance().RecordEvent(
      UserActivityEventType::kClosedTab);

  // Act

  // Assert
  EXPECT_EQ("120", predictor_variable->GetValue());
}

}  // namespace hns_ads
