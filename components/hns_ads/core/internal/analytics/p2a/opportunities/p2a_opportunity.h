/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ANALYTICS_P2A_OPPORTUNITIES_P2A_OPPORTUNITY_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ANALYTICS_P2A_OPPORTUNITIES_P2A_OPPORTUNITY_H_

#include "hns/components/hns_ads/core/internal/segments/segment_alias.h"

namespace hns_ads {

class AdType;

namespace p2a {

void RecordAdOpportunity(const AdType& ad_type, const SegmentList& segments);

}  // namespace p2a
}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ANALYTICS_P2A_OPPORTUNITIES_P2A_OPPORTUNITY_H_
