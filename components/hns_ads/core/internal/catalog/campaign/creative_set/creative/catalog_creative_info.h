/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_CATALOG_CAMPAIGN_CREATIVE_SET_CREATIVE_CATALOG_CREATIVE_INFO_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_CATALOG_CAMPAIGN_CREATIVE_SET_CREATIVE_CATALOG_CREATIVE_INFO_H_

#include <string>

#include "hns/components/hns_ads/core/internal/catalog/campaign/creative_set/creative/catalog_type_info.h"

namespace hns_ads {

struct CatalogCreativeInfo {
  bool operator==(const CatalogCreativeInfo&) const;
  bool operator!=(const CatalogCreativeInfo&) const;

  std::string instance_id;
  CatalogTypeInfo type;
};

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_CATALOG_CAMPAIGN_CREATIVE_SET_CREATIVE_CATALOG_CREATIVE_INFO_H_
