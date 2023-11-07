/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/user_interaction/ad_events/search_result_ads/search_result_ad_event_factory.h"

#include "base/check.h"
#include "hns/components/hns_ads/core/internal/creatives/search_result_ads/search_result_ad_info.h"
#include "hns/components/hns_ads/core/internal/user_interaction/ad_events/search_result_ads/search_result_ad_event_clicked.h"
#include "hns/components/hns_ads/core/internal/user_interaction/ad_events/search_result_ads/search_result_ad_event_served.h"
#include "hns/components/hns_ads/core/internal/user_interaction/ad_events/search_result_ads/search_result_ad_event_viewed.h"

namespace hns_ads {

std::unique_ptr<AdEventInterface<SearchResultAdInfo>>
SearchResultAdEventFactory::Build(
    const mojom::SearchResultAdEventType event_type) {
  CHECK(mojom::IsKnownEnumValue(event_type));

  switch (event_type) {
    case mojom::SearchResultAdEventType::kServed: {
      return std::make_unique<SearchResultAdEventServed>();
    }

    case mojom::SearchResultAdEventType::kViewed: {
      return std::make_unique<SearchResultAdEventViewed>();
    }

    case mojom::SearchResultAdEventType::kClicked: {
      return std::make_unique<SearchResultAdEventClicked>();
    }
  }
}

}  // namespace hns_ads
