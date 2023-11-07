/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_TARGETING_BEHAVIORAL_PURCHASE_INTENT_RESOURCE_PURCHASE_INTENT_INFO_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_TARGETING_BEHAVIORAL_PURCHASE_INTENT_RESOURCE_PURCHASE_INTENT_INFO_H_

#include <string>
#include <vector>

#include "base/types/expected.h"
#include "base/values.h"
#include "hns/components/hns_ads/core/internal/targeting/behavioral/purchase_intent/model/purchase_intent_funnel_keyword_info.h"
#include "hns/components/hns_ads/core/internal/targeting/behavioral/purchase_intent/resource/purchase_intent_segment_keyword_info.h"
#include "hns/components/hns_ads/core/internal/targeting/behavioral/purchase_intent/resource/purchase_intent_site_info.h"

namespace hns_ads {

struct PurchaseIntentInfo final {
  PurchaseIntentInfo();

  PurchaseIntentInfo(const PurchaseIntentInfo&) = delete;
  PurchaseIntentInfo& operator=(const PurchaseIntentInfo&) = delete;

  PurchaseIntentInfo(PurchaseIntentInfo&&) noexcept;
  PurchaseIntentInfo& operator=(PurchaseIntentInfo&&) noexcept;

  ~PurchaseIntentInfo();

  static base::expected<PurchaseIntentInfo, std::string> CreateFromValue(
      base::Value::Dict dict);

  int version = 0;
  std::vector<PurchaseIntentSiteInfo> sites;
  std::vector<PurchaseIntentSegmentKeywordInfo> segment_keywords;
  std::vector<PurchaseIntentFunnelKeywordInfo> funnel_keywords;
};

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_TARGETING_BEHAVIORAL_PURCHASE_INTENT_RESOURCE_PURCHASE_INTENT_INFO_H_
