/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_FL_PREDICTORS_VARIABLES_NOTIFICATION_AD_SERVED_AT_PREDICTOR_VARIABLE_UTIL_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_FL_PREDICTORS_VARIABLES_NOTIFICATION_AD_SERVED_AT_PREDICTOR_VARIABLE_UTIL_H_

namespace base {
class Time;
}  // namespace base

namespace hns_ads {

void SetNotificationAdServedAtPredictorVariable(base::Time time);

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_FL_PREDICTORS_VARIABLES_NOTIFICATION_AD_SERVED_AT_PREDICTOR_VARIABLE_UTIL_H_
