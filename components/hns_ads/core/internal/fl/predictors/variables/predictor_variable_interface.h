/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_FL_PREDICTORS_VARIABLES_PREDICTOR_VARIABLE_INTERFACE_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_FL_PREDICTORS_VARIABLES_PREDICTOR_VARIABLE_INTERFACE_H_

#include <string>

#include "hns/components/hns_federated/public/interfaces/hns_federated.mojom-shared.h"

namespace hns_ads {

class PredictorVariableInterface {
 public:
  virtual ~PredictorVariableInterface() = default;

  virtual hns_federated::mojom::DataType GetDataType() const = 0;
  virtual hns_federated::mojom::CovariateType GetType() const = 0;
  virtual std::string GetValue() const = 0;
};

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_FL_PREDICTORS_VARIABLES_PREDICTOR_VARIABLE_INTERFACE_H_
