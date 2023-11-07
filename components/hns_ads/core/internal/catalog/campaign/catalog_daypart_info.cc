/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/catalog/campaign/catalog_daypart_info.h"

#include <tuple>

namespace hns_ads {

bool operator==(const CatalogDaypartInfo& lhs, const CatalogDaypartInfo& rhs) {
  const auto tie = [](const CatalogDaypartInfo& daypart) {
    return std::tie(daypart.days_of_week, daypart.start_minute,
                    daypart.end_minute);
  };

  return tie(lhs) == tie(rhs);
}

bool operator!=(const CatalogDaypartInfo& lhs, const CatalogDaypartInfo& rhs) {
  return !(lhs == rhs);
}

}  // namespace hns_ads
