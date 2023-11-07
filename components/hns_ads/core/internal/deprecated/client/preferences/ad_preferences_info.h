/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_DEPRECATED_CLIENT_PREFERENCES_AD_PREFERENCES_INFO_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_DEPRECATED_CLIENT_PREFERENCES_AD_PREFERENCES_INFO_H_

#include <string>

#include "base/values.h"
#include "hns/components/hns_ads/core/internal/deprecated/client/preferences/filtered_advertiser_info.h"
#include "hns/components/hns_ads/core/internal/deprecated/client/preferences/filtered_category_info.h"
#include "hns/components/hns_ads/core/internal/deprecated/client/preferences/flagged_ad_info.h"
#include "hns/components/hns_ads/core/internal/deprecated/client/preferences/saved_ad_info.h"

namespace hns_ads {

struct AdPreferencesInfo final {
  AdPreferencesInfo();

  AdPreferencesInfo(const AdPreferencesInfo&);
  AdPreferencesInfo& operator=(const AdPreferencesInfo&);

  AdPreferencesInfo(AdPreferencesInfo&&) noexcept;
  AdPreferencesInfo& operator=(AdPreferencesInfo&&) noexcept;

  ~AdPreferencesInfo();

  base::Value::Dict ToValue() const;
  void FromValue(const base::Value::Dict& dict);

  std::string ToJson() const;
  [[nodiscard]] bool FromJson(const std::string& json);

  FilteredAdvertiserList filtered_advertisers;
  FilteredCategoryList filtered_categories;
  SavedAdList saved_ads;
  FlaggedAdList flagged_ads;
};

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_DEPRECATED_CLIENT_PREFERENCES_AD_PREFERENCES_INFO_H_
