/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_TABS_TAB_INFO_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_TABS_TAB_INFO_H_

#include <cstdint>
#include <vector>

#include "url/gurl.h"

namespace hns_ads {

struct TabInfo final {
  TabInfo();

  TabInfo(const TabInfo&);
  TabInfo& operator=(const TabInfo&);

  TabInfo(TabInfo&&) noexcept;
  TabInfo& operator=(TabInfo&&) noexcept;

  ~TabInfo();

  bool operator==(const TabInfo&) const;
  bool operator!=(const TabInfo&) const;

  int32_t id = 0;
  std::vector<GURL> redirect_chain;
  bool is_playing_media = false;
};

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_TABS_TAB_INFO_H_
