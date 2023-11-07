/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/creatives/search_result_ads/search_result_ad_info.h"

namespace hns_ads {

bool SearchResultAdInfo::IsValid() const {
  return AdInfo::IsValid() && !headline_text.empty() && !description.empty();
}

}  // namespace hns_ads
