/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/conversions/conversions_util.h"

#include "base/notreached.h"
#include "base/time/time.h"
#include "hns/components/hns_ads/core/internal/settings/settings.h"
#include "hns/components/hns_ads/core/internal/user_interaction/ad_events/ad_event_info.h"

namespace hns_ads {

bool CanConvertAdEvent(const AdEventInfo& ad_event) {
  // Only convert view-through and click-through ad events.
  if (ad_event.confirmation_type != ConfirmationType::kViewed &&
      ad_event.confirmation_type != ConfirmationType::kClicked) {
    return false;
  }

  switch (ad_event.type.value()) {
    case AdType::kInlineContentAd:
    case AdType::kPromotedContentAd: {
      return UserHasOptedInToHnsNewsAds();
    }

    case AdType::kNewTabPageAd: {
      return UserHasOptedInToNewTabPageAds();
    }

    case AdType::kNotificationAd: {
      return UserHasOptedInToNotificationAds();
    }

    case AdType::kSearchResultAd: {
      // Always.
      return true;
    }

    case AdType::kUndefined: {
      NOTREACHED_NORETURN();
    }
  }

  NOTREACHED_NORETURN() << "Unexpected value for AdType: "
                        << static_cast<int>(ad_event.type.value());
}

bool HasObservationWindowForAdEventExpired(
    const base::TimeDelta observation_window,
    const AdEventInfo& ad_event) {
  return ad_event.created_at < base::Time::Now() - observation_window;
}

}  // namespace hns_ads
