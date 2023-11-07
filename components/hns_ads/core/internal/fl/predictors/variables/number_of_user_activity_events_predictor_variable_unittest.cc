/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/fl/predictors/variables/number_of_user_activity_events_predictor_variable.h"

#include <memory>

#include "hns/components/hns_ads/core/internal/common/unittest/unittest_base.h"
#include "hns/components/hns_ads/core/internal/user_attention/user_activity/user_activity_manager.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads {

class HnsAdsNumberOfUserActivityEventsPredictorVariableTest
    : public UnitTestBase {};

TEST_F(HnsAdsNumberOfUserActivityEventsPredictorVariableTest, GetDataType) {
  // Arrange
  std::unique_ptr<PredictorVariableInterface> predictor_variable =
      std::make_unique<NumberOfUserActivityEventsPredictorVariable>(
          UserActivityEventType::kOpenedNewTab,
          hns_federated::mojom::CovariateType::kNumberOfOpenedNewTabEvents);

  // Act

  // Assert
  EXPECT_EQ(hns_federated::mojom::DataType::kInt,
            predictor_variable->GetDataType());
}

TEST_F(HnsAdsNumberOfUserActivityEventsPredictorVariableTest,
       GetValueWithoutUserActivity) {
  // Arrange
  std::unique_ptr<PredictorVariableInterface> predictor_variable =
      std::make_unique<NumberOfUserActivityEventsPredictorVariable>(
          UserActivityEventType::kOpenedNewTab,
          hns_federated::mojom::CovariateType::kNumberOfOpenedNewTabEvents);

  // Act

  // Assert
  EXPECT_EQ("0", predictor_variable->GetValue());
}

TEST_F(HnsAdsNumberOfUserActivityEventsPredictorVariableTest, GetValue) {
  // Arrange
  std::unique_ptr<PredictorVariableInterface> predictor_variable =
      std::make_unique<NumberOfUserActivityEventsPredictorVariable>(
          UserActivityEventType::kOpenedNewTab,
          hns_federated::mojom::CovariateType::kNumberOfOpenedNewTabEvents);

  UserActivityManager::GetInstance().RecordEvent(
      UserActivityEventType::kOpenedNewTab);
  UserActivityManager::GetInstance().RecordEvent(
      UserActivityEventType::kBrowserDidResignActive);

  AdvanceClockBy(base::Minutes(31));

  UserActivityManager::GetInstance().RecordEvent(
      UserActivityEventType::kBrowserDidBecomeActive);
  UserActivityManager::GetInstance().RecordEvent(
      UserActivityEventType::kOpenedNewTab);
  UserActivityManager::GetInstance().RecordEvent(
      UserActivityEventType::kClosedTab);
  UserActivityManager::GetInstance().RecordEvent(
      UserActivityEventType::kOpenedNewTab);

  // Act

  // Assert
  EXPECT_EQ("2", predictor_variable->GetValue());
}

}  // namespace hns_ads
