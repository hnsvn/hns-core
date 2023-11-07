/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_SERVING_TARGETING_TOP_SEGMENTS_UTIL_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_SERVING_TARGETING_TOP_SEGMENTS_UTIL_H_

#include "hns/components/hns_ads/core/internal/segments/segment_alias.h"

namespace hns_ads {

struct UserModelInfo;

SegmentList GetTopSegments(const SegmentList& segments,
                           int max_count,
                           bool parent_only);

SegmentList GetTopSegments(const UserModelInfo& user_model,
                           int max_count,
                           bool parent_only);

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_SERVING_TARGETING_TOP_SEGMENTS_UTIL_H_
