/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_USER_INTERACTION_AD_EVENTS_SEARCH_RESULT_ADS_SEARCH_RESULT_AD_EVENT_HANDLER_DELEGATE_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_USER_INTERACTION_AD_EVENTS_SEARCH_RESULT_ADS_SEARCH_RESULT_AD_EVENT_HANDLER_DELEGATE_H_

#include "hns/components/hns_ads/core/mojom/hns_ads.mojom-shared.h"

namespace hns_ads {

struct SearchResultAdInfo;

class SearchResultAdEventHandlerDelegate {
 public:
  // Invoked when the search result |ad| is served.
  virtual void OnDidFireSearchResultAdServedEvent(
      const SearchResultAdInfo& ad) {}

  // Invoked when the search result |ad| is viewed.
  virtual void OnDidFireSearchResultAdViewedEvent(
      const SearchResultAdInfo& ad) {}

  // Invoked when the search result |ad| is clicked.
  virtual void OnDidFireSearchResultAdClickedEvent(
      const SearchResultAdInfo& ad) {}

  // Invoked when the search result |ad| event fails for |event_type|.
  virtual void OnFailedToFireSearchResultAdEvent(
      const SearchResultAdInfo& ad,
      const mojom::SearchResultAdEventType event_type) {}

 protected:
  virtual ~SearchResultAdEventHandlerDelegate() = default;
};

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_USER_INTERACTION_AD_EVENTS_SEARCH_RESULT_ADS_SEARCH_RESULT_AD_EVENT_HANDLER_DELEGATE_H_
