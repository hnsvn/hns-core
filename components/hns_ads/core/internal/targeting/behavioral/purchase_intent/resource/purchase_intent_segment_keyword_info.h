/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_TARGETING_BEHAVIORAL_PURCHASE_INTENT_RESOURCE_PURCHASE_INTENT_SEGMENT_KEYWORD_INFO_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_TARGETING_BEHAVIORAL_PURCHASE_INTENT_RESOURCE_PURCHASE_INTENT_SEGMENT_KEYWORD_INFO_H_

#include <string>

#include "hns/components/hns_ads/core/internal/segments/segment_alias.h"

namespace hns_ads {

struct PurchaseIntentSegmentKeywordInfo final {
  PurchaseIntentSegmentKeywordInfo();
  PurchaseIntentSegmentKeywordInfo(SegmentList segments, std::string keywords);

  PurchaseIntentSegmentKeywordInfo(const PurchaseIntentSegmentKeywordInfo&);
  PurchaseIntentSegmentKeywordInfo& operator=(
      const PurchaseIntentSegmentKeywordInfo&);

  PurchaseIntentSegmentKeywordInfo(PurchaseIntentSegmentKeywordInfo&&) noexcept;
  PurchaseIntentSegmentKeywordInfo& operator=(
      PurchaseIntentSegmentKeywordInfo&&) noexcept;

  ~PurchaseIntentSegmentKeywordInfo();

  SegmentList segments;
  std::string keywords;
};

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_TARGETING_BEHAVIORAL_PURCHASE_INTENT_RESOURCE_PURCHASE_INTENT_SEGMENT_KEYWORD_INFO_H_
