/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_SEGMENTS_SEGMENT_VALUE_UTIL_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_SEGMENTS_SEGMENT_VALUE_UTIL_H_

#include "base/values.h"
#include "hns/components/hns_ads/core/internal/segments/segment_alias.h"

namespace hns_ads {

base::Value::List SegmentsToValue(const SegmentList& segments);
SegmentList SegmentsFromValue(const base::Value::List& list);

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_SEGMENTS_SEGMENT_VALUE_UTIL_H_
