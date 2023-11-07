/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_CATALOG_CAMPAIGN_CREATIVE_SET_CATALOG_OS_INFO_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_CATALOG_CAMPAIGN_CREATIVE_SET_CATALOG_OS_INFO_H_

#include <string>
#include <vector>

namespace hns_ads {

struct CatalogOsInfo final {
  std::string code;
  std::string name;
};

bool operator==(const CatalogOsInfo&, const CatalogOsInfo&);
bool operator!=(const CatalogOsInfo&, const CatalogOsInfo&);

using CatalogOsList = std::vector<CatalogOsInfo>;

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_CATALOG_CAMPAIGN_CREATIVE_SET_CATALOG_OS_INFO_H_
