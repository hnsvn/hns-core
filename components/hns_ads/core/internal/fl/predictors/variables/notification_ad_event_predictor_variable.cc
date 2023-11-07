/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/fl/predictors/variables/notification_ad_event_predictor_variable.h"

#include <sstream>

#include "base/check.h"

namespace hns_ads {

NotificationAdEventPredictorVariable::NotificationAdEventPredictorVariable(
    const mojom::NotificationAdEventType event_type)
    : event_type_(event_type) {
  CHECK(mojom::IsKnownEnumValue(event_type_));
}

hns_federated::mojom::DataType
NotificationAdEventPredictorVariable::GetDataType() const {
  return hns_federated::mojom::DataType::kString;
}

hns_federated::mojom::CovariateType
NotificationAdEventPredictorVariable::GetType() const {
  return hns_federated::mojom::CovariateType::kNotificationAdEvent;
}

std::string NotificationAdEventPredictorVariable::GetValue() const {
  std::stringstream ss;
  ss << event_type_;
  return ss.str();
}

}  // namespace hns_ads
