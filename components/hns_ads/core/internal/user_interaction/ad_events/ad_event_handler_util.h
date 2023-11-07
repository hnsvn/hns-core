/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_USER_INTERACTION_AD_EVENTS_AD_EVENT_HANDLER_UTIL_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_USER_INTERACTION_AD_EVENTS_AD_EVENT_HANDLER_UTIL_H_

#include "hns/components/hns_ads/core/internal/user_interaction/ad_events/ad_event_info.h"

namespace hns_ads {

class ConfirmationType;
struct AdInfo;

bool HasFiredAdEvent(const AdInfo& ad,
                     const AdEventList& ad_events,
                     const ConfirmationType& confirmation_type);

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_USER_INTERACTION_AD_EVENTS_AD_EVENT_HANDLER_UTIL_H_
