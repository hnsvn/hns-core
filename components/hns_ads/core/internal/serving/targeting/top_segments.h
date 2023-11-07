/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_SERVING_TARGETING_TOP_SEGMENTS_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_SERVING_TARGETING_TOP_SEGMENTS_H_

#include "hns/components/hns_ads/core/internal/segments/segment_alias.h"

namespace hns_ads {

struct UserModelInfo;

SegmentList GetTopChildSegments(const UserModelInfo& user_model);
SegmentList GetTopParentSegments(const UserModelInfo& user_model);

SegmentList GetTopChildInterestSegments(const UserModelInfo& user_model);
SegmentList GetTopParentInterestSegments(const UserModelInfo& user_model);

SegmentList GetTopChildLatentInterestSegments(const UserModelInfo& user_model);
SegmentList GetTopParentLatentInterestSegments(const UserModelInfo& user_model);

SegmentList GetTopChildIntentSegments(const UserModelInfo& user_model);
SegmentList GetTopParentIntentSegments(const UserModelInfo& user_model);

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_SERVING_TARGETING_TOP_SEGMENTS_H_
