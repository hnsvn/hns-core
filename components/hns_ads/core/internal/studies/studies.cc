/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/studies/studies.h"

#include "hns/components/hns_ads/core/internal/client/ads_client_helper.h"
#include "hns/components/hns_ads/core/internal/studies/studies_util.h"

namespace hns_ads {

Studies::Studies() {
  AdsClientHelper::AddObserver(this);
}

Studies::~Studies() {
  AdsClientHelper::RemoveObserver(this);
}

///////////////////////////////////////////////////////////////////////////////

void Studies::OnNotifyDidInitializeAds() {
  LogActiveStudies();
}

}  // namespace hns_ads
