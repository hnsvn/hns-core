/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_CATALOG_CAMPAIGN_CATALOG_DAYPART_INFO_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_CATALOG_CAMPAIGN_CATALOG_DAYPART_INFO_H_

#include <string>
#include <vector>

#include "base/time/time.h"

namespace hns_ads {

struct CatalogDaypartInfo final {
  std::string days_of_week =
      "0123456";  // Sunday=0, Monday=1, Tuesday=2, Wednesday=3, Thursday=4,
                  // Friday=5 and Saturday=6
  int start_minute = 0;                                             // 00:00
  int end_minute = (base::Days(1) - base::Minutes(1)).InMinutes();  // 23:59
};

bool operator==(const CatalogDaypartInfo&, const CatalogDaypartInfo&);
bool operator!=(const CatalogDaypartInfo&, const CatalogDaypartInfo&);

using CatalogDaypartList = std::vector<CatalogDaypartInfo>;

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_CATALOG_CAMPAIGN_CATALOG_DAYPART_INFO_H_
