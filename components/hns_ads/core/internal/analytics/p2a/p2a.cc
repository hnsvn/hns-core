/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/analytics/p2a/p2a.h"

#include "hns/components/hns_ads/core/internal/analytics/p2a/p2a_value_util.h"
#include "hns/components/hns_ads/core/internal/client/ads_client_helper.h"

namespace hns_ads::p2a {

void RecordEvent(const std::vector<std::string>& events) {
  CHECK(!events.empty());

  AdsClientHelper::GetInstance()->RecordP2AEvents(EventsToValue(events));
}

}  // namespace hns_ads::p2a
