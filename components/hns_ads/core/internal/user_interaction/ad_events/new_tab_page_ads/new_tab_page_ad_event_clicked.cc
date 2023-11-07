/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/user_interaction/ad_events/new_tab_page_ads/new_tab_page_ad_event_clicked.h"

#include <utility>

#include "hns/components/hns_ads/core/internal/user_interaction/ad_events/ad_events.h"
#include "hns/components/hns_ads/core/public/ads/new_tab_page_ad_info.h"
#include "hns/components/hns_ads/core/public/confirmation_type.h"

namespace hns_ads {

void NewTabPageAdEventClicked::FireEvent(const NewTabPageAdInfo& ad,
                                         ResultCallback callback) {
  LogAdEvent(ad, ConfirmationType::kClicked, std::move(callback));
}

}  // namespace hns_ads
