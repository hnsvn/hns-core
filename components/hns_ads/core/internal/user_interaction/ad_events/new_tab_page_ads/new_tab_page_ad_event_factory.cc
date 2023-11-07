/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/user_interaction/ad_events/new_tab_page_ads/new_tab_page_ad_event_factory.h"

#include "base/check.h"
#include "hns/components/hns_ads/core/internal/user_interaction/ad_events/new_tab_page_ads/new_tab_page_ad_event_clicked.h"
#include "hns/components/hns_ads/core/internal/user_interaction/ad_events/new_tab_page_ads/new_tab_page_ad_event_served.h"
#include "hns/components/hns_ads/core/internal/user_interaction/ad_events/new_tab_page_ads/new_tab_page_ad_event_viewed.h"
#include "hns/components/hns_ads/core/public/ads/new_tab_page_ad_info.h"

namespace hns_ads {

std::unique_ptr<AdEventInterface<NewTabPageAdInfo>>
NewTabPageAdEventFactory::Build(const mojom::NewTabPageAdEventType event_type) {
  CHECK(mojom::IsKnownEnumValue(event_type));

  switch (event_type) {
    case mojom::NewTabPageAdEventType::kServed: {
      return std::make_unique<NewTabPageAdEventServed>();
    }

    case mojom::NewTabPageAdEventType::kViewed: {
      return std::make_unique<NewTabPageAdEventViewed>();
    }

    case mojom::NewTabPageAdEventType::kClicked: {
      return std::make_unique<NewTabPageAdEventClicked>();
    }
  }
}

}  // namespace hns_ads
