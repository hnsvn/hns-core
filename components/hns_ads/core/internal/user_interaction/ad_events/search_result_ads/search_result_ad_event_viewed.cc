/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/user_interaction/ad_events/search_result_ads/search_result_ad_event_viewed.h"

#include <utility>

#include "hns/components/hns_ads/core/internal/creatives/search_result_ads/search_result_ad_info.h"
#include "hns/components/hns_ads/core/internal/user_interaction/ad_events/ad_events.h"
#include "hns/components/hns_ads/core/public/confirmation_type.h"

namespace hns_ads {

void SearchResultAdEventViewed::FireEvent(const SearchResultAdInfo& ad,
                                          ResultCallback callback) {
  LogAdEvent(ad, ConfirmationType::kViewed, std::move(callback));
}

}  // namespace hns_ads
