/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_FL_PREDICTORS_VARIABLES_TIME_SINCE_LAST_USER_ACTIVITY_EVENT_PREDICTOR_VARIABLE_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_FL_PREDICTORS_VARIABLES_TIME_SINCE_LAST_USER_ACTIVITY_EVENT_PREDICTOR_VARIABLE_H_

#include <string>

#include "hns/components/hns_ads/core/internal/fl/predictors/variables/predictor_variable_interface.h"
#include "hns/components/hns_ads/core/internal/user_attention/user_activity/user_activity_event_types.h"

namespace hns_ads {

class TimeSinceLastUserActivityEventPredictorVariable final
    : public PredictorVariableInterface {
 public:
  TimeSinceLastUserActivityEventPredictorVariable(
      UserActivityEventType event_type,
      hns_federated::mojom::CovariateType predictor_type);

  // PredictorVariableInterface:
  hns_federated::mojom::DataType GetDataType() const override;
  hns_federated::mojom::CovariateType GetType() const override;
  std::string GetValue() const override;

 private:
  const UserActivityEventType event_type_;
  const hns_federated::mojom::CovariateType predictor_type_;
};

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_FL_PREDICTORS_VARIABLES_TIME_SINCE_LAST_USER_ACTIVITY_EVENT_PREDICTOR_VARIABLE_H_
