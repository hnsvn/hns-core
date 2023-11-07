/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/fl/predictors/variables/notification_ad_served_at_predictor_variable.h"

#include "base/strings/string_number_conversions.h"
#include "hns/components/hns_ads/core/internal/fl/predictors/variables/predictor_variable_constants.h"

namespace hns_ads {

NotificationAdServedAtPredictorVariable::
    NotificationAdServedAtPredictorVariable(const base::Time time)
    : time_(time) {}

hns_federated::mojom::DataType
NotificationAdServedAtPredictorVariable::GetDataType() const {
  return hns_federated::mojom::DataType::kDouble;
}

hns_federated::mojom::CovariateType
NotificationAdServedAtPredictorVariable::GetType() const {
  return hns_federated::mojom::CovariateType::kNotificationAdServedAt;
}

std::string NotificationAdServedAtPredictorVariable::GetValue() const {
  return base::NumberToString(
      time_.is_null() ? kPredictorVariableMissingValue
                      : time_.ToDeltaSinceWindowsEpoch().InMicroseconds());
}

}  // namespace hns_ads
