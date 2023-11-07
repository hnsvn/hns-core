/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_CATALOG_CATALOG_INFO_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_CATALOG_CATALOG_INFO_H_

#include <string>

#include "base/time/time.h"
#include "hns/components/hns_ads/core/internal/catalog/campaign/catalog_campaign_info.h"

namespace hns_ads {

struct CatalogInfo final {
  CatalogInfo();

  CatalogInfo(const CatalogInfo&);
  CatalogInfo& operator=(const CatalogInfo&);

  CatalogInfo(CatalogInfo&&) noexcept;
  CatalogInfo& operator=(CatalogInfo&&) noexcept;

  ~CatalogInfo();

  bool operator==(const CatalogInfo&) const;
  bool operator!=(const CatalogInfo&) const;

  std::string id;
  int version = 0;
  base::TimeDelta ping;
  CatalogCampaignList campaigns;
};

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_CATALOG_CATALOG_INFO_H_
