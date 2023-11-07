/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/user_interaction/ad_events/notification_ads/notification_ad_event_factory.h"

#include "base/check.h"
#include "hns/components/hns_ads/core/internal/user_interaction/ad_events/notification_ads/notification_ad_event_clicked.h"
#include "hns/components/hns_ads/core/internal/user_interaction/ad_events/notification_ads/notification_ad_event_dismissed.h"
#include "hns/components/hns_ads/core/internal/user_interaction/ad_events/notification_ads/notification_ad_event_served.h"
#include "hns/components/hns_ads/core/internal/user_interaction/ad_events/notification_ads/notification_ad_event_timed_out.h"
#include "hns/components/hns_ads/core/internal/user_interaction/ad_events/notification_ads/notification_ad_event_viewed.h"
#include "hns/components/hns_ads/core/public/ads/notification_ad_info.h"

namespace hns_ads {

std::unique_ptr<AdEventInterface<NotificationAdInfo>>
NotificationAdEventFactory::Build(
    const mojom::NotificationAdEventType event_type) {
  CHECK(mojom::IsKnownEnumValue(event_type));

  switch (event_type) {
    case mojom::NotificationAdEventType::kServed: {
      return std::make_unique<NotificationAdEventServed>();
    }

    case mojom::NotificationAdEventType::kViewed: {
      return std::make_unique<NotificationAdEventViewed>();
    }

    case mojom::NotificationAdEventType::kClicked: {
      return std::make_unique<NotificationAdEventClicked>();
    }

    case mojom::NotificationAdEventType::kDismissed: {
      return std::make_unique<NotificationAdEventDismissed>();
    }

    case mojom::NotificationAdEventType::kTimedOut: {
      return std::make_unique<NotificationAdEventTimedOut>();
    }
  }
}

}  // namespace hns_ads
