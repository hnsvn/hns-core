/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_PUBLIC_ADS_NEW_TAB_PAGE_AD_INFO_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_PUBLIC_ADS_NEW_TAB_PAGE_AD_INFO_H_

#include <string>

#include "hns/components/hns_ads/core/public/ad_info.h"
#include "hns/components/hns_ads/core/public/ads/new_tab_page_ad_wallpaper_info.h"
#include "hns/components/hns_ads/core/public/export.h"
#include "url/gurl.h"

namespace hns_ads {

struct ADS_EXPORT NewTabPageAdInfo final : AdInfo {
  NewTabPageAdInfo();

  NewTabPageAdInfo(const NewTabPageAdInfo&);
  NewTabPageAdInfo& operator=(const NewTabPageAdInfo&);

  NewTabPageAdInfo(NewTabPageAdInfo&&) noexcept;
  NewTabPageAdInfo& operator=(NewTabPageAdInfo&&) noexcept;

  ~NewTabPageAdInfo();

  bool operator==(const NewTabPageAdInfo&) const;
  bool operator!=(const NewTabPageAdInfo&) const;

  [[nodiscard]] bool IsValid() const;

  std::string company_name;
  GURL image_url;
  std::string alt;
  NewTabPageAdWallpaperList wallpapers;
};

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_PUBLIC_ADS_NEW_TAB_PAGE_AD_INFO_H_
