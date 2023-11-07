/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_USER_INTERACTION_AD_EVENTS_NEW_TAB_PAGE_ADS_NEW_TAB_PAGE_AD_EVENT_SERVED_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_USER_INTERACTION_AD_EVENTS_NEW_TAB_PAGE_ADS_NEW_TAB_PAGE_AD_EVENT_SERVED_H_

#include "hns/components/hns_ads/core/internal/user_interaction/ad_events/ad_event_interface.h"

namespace hns_ads {

struct NewTabPageAdInfo;

class NewTabPageAdEventServed final
    : public AdEventInterface<NewTabPageAdInfo> {
 public:
  void FireEvent(const NewTabPageAdInfo& ad, ResultCallback callback) override;
};

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_USER_INTERACTION_AD_EVENTS_NEW_TAB_PAGE_ADS_NEW_TAB_PAGE_AD_EVENT_SERVED_H_
