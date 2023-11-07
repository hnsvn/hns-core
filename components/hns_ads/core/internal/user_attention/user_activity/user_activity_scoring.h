/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_USER_ATTENTION_USER_ACTIVITY_USER_ACTIVITY_SCORING_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_USER_ATTENTION_USER_ACTIVITY_USER_ACTIVITY_SCORING_H_

#include "hns/components/hns_ads/core/internal/user_attention/user_activity/user_activity_event_info.h"
#include "hns/components/hns_ads/core/internal/user_attention/user_activity/user_activity_trigger_info.h"

namespace hns_ads {

double GetUserActivityScore(const UserActivityTriggerList& triggers,
                            const UserActivityEventList& events);

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_USER_ATTENTION_USER_ACTIVITY_USER_ACTIVITY_SCORING_H_
