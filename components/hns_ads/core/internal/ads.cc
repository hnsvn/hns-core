/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/public/ads.h"

#include "base/check.h"
#include "hns/components/hns_ads/core/internal/ads_impl.h"

namespace hns_ads {

// static
Ads* Ads::CreateInstance(AdsClient* ads_client) {
  CHECK(ads_client);
  return new AdsImpl(ads_client);
}

}  // namespace hns_ads
