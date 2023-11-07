/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/fl/predictors/variables/notification_ad_served_at_predictor_variable.h"

#include <memory>

#include "hns/components/hns_ads/core/internal/common/unittest/unittest_base.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_time_util.h"

// npm run test -- hns_unit_tests --filter=HnsAds*

namespace hns_ads {

class HnsAdsNotificationAdServedAtPredictorVariableTest
    : public UnitTestBase {};

TEST_F(HnsAdsNotificationAdServedAtPredictorVariableTest, GetDataType) {
  std::unique_ptr<PredictorVariableInterface> predictor_variable =
      std::make_unique<NotificationAdServedAtPredictorVariable>(
          base::Time::Now());

  // Act

  // Assert
  EXPECT_EQ(hns_federated::mojom::DataType::kDouble,
            predictor_variable->GetDataType());
}

TEST_F(HnsAdsNotificationAdServedAtPredictorVariableTest, GetValue) {
  // Arrange
  const base::Time now = TimeFromString("August 19 2019", /*is_local*/ false);
  std::unique_ptr<PredictorVariableInterface> predictor_variable =
      std::make_unique<NotificationAdServedAtPredictorVariable>(now);

  // Act

  // Assert
  EXPECT_EQ("13210646400000000", predictor_variable->GetValue());
}

TEST_F(HnsAdsNotificationAdServedAtPredictorVariableTest,
       GetValueWithoutTime) {
  // Arrange
  std::unique_ptr<PredictorVariableInterface> predictor_variable =
      std::make_unique<NotificationAdServedAtPredictorVariable>(base::Time());

  // Act

  // Assert
  EXPECT_EQ("-1", predictor_variable->GetValue());
}

}  // namespace hns_ads
