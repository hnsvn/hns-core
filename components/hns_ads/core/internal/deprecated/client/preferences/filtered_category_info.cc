/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/deprecated/client/preferences/filtered_category_info.h"

namespace hns_ads {

base::Value::Dict FilteredCategoryInfo::ToValue() const {
  base::Value::Dict dict;
  dict.Set("name", name);
  return dict;
}

void FilteredCategoryInfo::FromValue(const base::Value::Dict& dict) {
  if (const auto* const value = dict.FindString("name")) {
    name = *value;
  }
}

}  // namespace hns_ads
