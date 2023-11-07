/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_USER_INTERACTION_AD_EVENTS_AD_EVENT_BUILDER_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_USER_INTERACTION_AD_EVENTS_AD_EVENT_BUILDER_H_

namespace base {
class Time;
}  // namespace base

namespace hns_ads {

class ConfirmationType;
struct AdEventInfo;
struct AdInfo;

AdEventInfo BuildAdEvent(const AdInfo& ad,
                         const ConfirmationType& confirmation_type,
                         base::Time created_at);

AdEventInfo RebuildAdEvent(const AdEventInfo& ad_event,
                           const ConfirmationType& confirmation_type,
                           base::Time created_at);

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_USER_INTERACTION_AD_EVENTS_AD_EVENT_BUILDER_H_
