/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_CREATIVES_NEW_TAB_PAGE_ADS_CREATIVE_NEW_TAB_PAGE_ADS_DATABASE_UTIL_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_CREATIVES_NEW_TAB_PAGE_ADS_CREATIVE_NEW_TAB_PAGE_ADS_DATABASE_UTIL_H_

#include "hns/components/hns_ads/core/internal/creatives/new_tab_page_ads/creative_new_tab_page_ad_info.h"

namespace hns_ads::database {

void DeleteCreativeNewTabPageAds();
void DeleteCreativeNewTabPageAdWallpapers();

void SaveCreativeNewTabPageAds(const CreativeNewTabPageAdList& creative_ads);

}  // namespace hns_ads::database

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_CREATIVES_NEW_TAB_PAGE_ADS_CREATIVE_NEW_TAB_PAGE_ADS_DATABASE_UTIL_H_
