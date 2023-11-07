/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_FL_PREDICTORS_VARIABLES_NOTIFICATION_AD_EVENT_PREDICTOR_VARIABLE_UTIL_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_FL_PREDICTORS_VARIABLES_NOTIFICATION_AD_EVENT_PREDICTOR_VARIABLE_UTIL_H_

#include "hns/components/hns_ads/core/mojom/hns_ads.mojom-shared.h"

namespace hns_ads {

void SetNotificationAdEventPredictorVariable(
    mojom::NotificationAdEventType event_type);

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_FL_PREDICTORS_VARIABLES_NOTIFICATION_AD_EVENT_PREDICTOR_VARIABLE_UTIL_H_
