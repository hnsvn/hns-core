/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_CATALOG_CAMPAIGN_CREATIVE_SET_CREATIVE_PROMOTED_CONTENT_AD_CATALOG_PROMOTED_CONTENT_AD_PAYLOAD_INFO_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_CATALOG_CAMPAIGN_CREATIVE_SET_CREATIVE_PROMOTED_CONTENT_AD_CATALOG_PROMOTED_CONTENT_AD_PAYLOAD_INFO_H_

#include <string>

#include "url/gurl.h"

namespace hns_ads {

struct CatalogPromotedContentAdPayloadInfo final {
  std::string title;
  std::string description;
  GURL target_url;
};

bool operator==(const CatalogPromotedContentAdPayloadInfo&,
                const CatalogPromotedContentAdPayloadInfo&);
bool operator!=(const CatalogPromotedContentAdPayloadInfo&,
                const CatalogPromotedContentAdPayloadInfo&);

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_CATALOG_CAMPAIGN_CREATIVE_SET_CREATIVE_PROMOTED_CONTENT_AD_CATALOG_PROMOTED_CONTENT_AD_PAYLOAD_INFO_H_
