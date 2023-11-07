/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/deprecated/client/preferences/filtered_advertiser_info.h"

namespace hns_ads {

base::Value::Dict FilteredAdvertiserInfo::ToValue() const {
  base::Value::Dict dict;
  dict.Set("id", id);
  return dict;
}

void FilteredAdvertiserInfo::FromValue(const base::Value::Dict& dict) {
  if (const auto* const value = dict.FindString("id")) {
    id = *value;
  }
}

}  // namespace hns_ads
