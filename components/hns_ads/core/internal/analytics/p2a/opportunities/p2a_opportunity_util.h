/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ANALYTICS_P2A_OPPORTUNITIES_P2A_OPPORTUNITY_UTIL_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ANALYTICS_P2A_OPPORTUNITIES_P2A_OPPORTUNITY_UTIL_H_

#include <string>
#include <vector>

#include "hns/components/hns_ads/core/internal/segments/segment_alias.h"

namespace hns_ads::p2a {

std::vector<std::string> BuildAdOpportunityEvents(const SegmentList& segments);

}  // namespace hns_ads::p2a

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ANALYTICS_P2A_OPPORTUNITIES_P2A_OPPORTUNITY_UTIL_H_
