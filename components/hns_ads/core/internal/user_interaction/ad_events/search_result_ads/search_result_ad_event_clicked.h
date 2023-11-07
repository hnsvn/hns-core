/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_USER_INTERACTION_AD_EVENTS_SEARCH_RESULT_ADS_SEARCH_RESULT_AD_EVENT_CLICKED_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_USER_INTERACTION_AD_EVENTS_SEARCH_RESULT_ADS_SEARCH_RESULT_AD_EVENT_CLICKED_H_

#include "hns/components/hns_ads/core/internal/user_interaction/ad_events/ad_event_interface.h"

namespace hns_ads {

struct SearchResultAdInfo;

class SearchResultAdEventClicked final
    : public AdEventInterface<SearchResultAdInfo> {
 public:
  void FireEvent(const SearchResultAdInfo& ad,
                 ResultCallback callback) override;
};

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_USER_INTERACTION_AD_EVENTS_SEARCH_RESULT_ADS_SEARCH_RESULT_AD_EVENT_CLICKED_H_
