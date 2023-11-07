/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_CREATIVES_NEW_TAB_PAGE_ADS_CREATIVE_NEW_TAB_PAGE_AD_UNITTEST_UTIL_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_CREATIVES_NEW_TAB_PAGE_ADS_CREATIVE_NEW_TAB_PAGE_AD_UNITTEST_UTIL_H_

#include "hns/components/hns_ads/core/internal/creatives/new_tab_page_ads/creative_new_tab_page_ad_info.h"

namespace hns_ads {

CreativeNewTabPageAdList BuildCreativeNewTabPageAdsForTesting(int count);
CreativeNewTabPageAdInfo BuildCreativeNewTabPageAdForTesting(
    bool should_use_random_uuids);

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_CREATIVES_NEW_TAB_PAGE_ADS_CREATIVE_NEW_TAB_PAGE_AD_UNITTEST_UTIL_H_
