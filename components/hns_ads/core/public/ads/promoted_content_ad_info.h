/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_PUBLIC_ADS_PROMOTED_CONTENT_AD_INFO_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_PUBLIC_ADS_PROMOTED_CONTENT_AD_INFO_H_

#include <string>

#include "hns/components/hns_ads/core/public/ad_info.h"
#include "hns/components/hns_ads/core/public/export.h"

namespace hns_ads {

struct ADS_EXPORT PromotedContentAdInfo final : AdInfo {
  [[nodiscard]] bool IsValid() const;

  std::string title;
  std::string description;
};

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_PUBLIC_ADS_PROMOTED_CONTENT_AD_INFO_H_
