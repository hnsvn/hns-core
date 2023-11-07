/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_FL_PREDICTORS_VARIABLES_LAST_NOTIFICATION_AD_WAS_CLICKED_PREDICTOR_VARIABLE_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_FL_PREDICTORS_VARIABLES_LAST_NOTIFICATION_AD_WAS_CLICKED_PREDICTOR_VARIABLE_H_

#include <string>

#include "hns/components/hns_ads/core/internal/fl/predictors/variables/predictor_variable_interface.h"

namespace hns_ads {

class LastNotificationAdWasClickedPredictorVariable final
    : public PredictorVariableInterface {
 public:
  // PredictorVariableInterface:
  hns_federated::mojom::DataType GetDataType() const override;
  hns_federated::mojom::CovariateType GetType() const override;
  std::string GetValue() const override;
};

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_FL_PREDICTORS_VARIABLES_LAST_NOTIFICATION_AD_WAS_CLICKED_PREDICTOR_VARIABLE_H_
