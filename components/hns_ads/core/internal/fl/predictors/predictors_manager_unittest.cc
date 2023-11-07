/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/fl/predictors/predictors_manager.h"

#include "hns/components/hns_ads/core/internal/common/unittest/unittest_base.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_time_util.h"
#include "hns/components/hns_ads/core/internal/fl/predictors/variables/notification_ad_event_predictor_variable_util.h"
#include "hns/components/hns_ads/core/internal/fl/predictors/variables/notification_ad_served_at_predictor_variable_util.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads {

class HnsAdsPredictorsManagerTest : public UnitTestBase {};

TEST_F(HnsAdsPredictorsManagerTest, GetTrainingSample) {
  // Arrange

  // Act
  const std::vector<hns_federated::mojom::CovariateInfoPtr> training_sample =
      PredictorsManager::GetInstance().GetTrainingSample();

  // Assert
  EXPECT_EQ(32U, training_sample.size());
}

TEST_F(HnsAdsPredictorsManagerTest, GetTrainingSampleWithSetters) {
  // Arrange
  SetNotificationAdServedAtPredictorVariable(Now());

  SetNotificationAdEventPredictorVariable(
      mojom::NotificationAdEventType::kClicked);

  // Act
  const std::vector<hns_federated::mojom::CovariateInfoPtr> training_sample =
      PredictorsManager::GetInstance().GetTrainingSample();

  // Assert
  EXPECT_EQ(34U, training_sample.size());
}

}  // namespace hns_ads
