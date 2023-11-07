/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/analytics/p2a/opportunities/p2a_opportunity.h"

#include "hns/components/hns_ads/core/internal/analytics/p2a/opportunities/p2a_opportunity_util.h"
#include "hns/components/hns_ads/core/internal/analytics/p2a/p2a.h"

namespace hns_ads::p2a {

void RecordAdOpportunity(const AdType& /*ad_type*/,
                         const SegmentList& segments) {
  RecordEvent(BuildAdOpportunityEvents(segments));
}

}  // namespace hns_ads::p2a
