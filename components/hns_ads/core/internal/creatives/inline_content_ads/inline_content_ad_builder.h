/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_CREATIVES_INLINE_CONTENT_ADS_INLINE_CONTENT_AD_BUILDER_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_CREATIVES_INLINE_CONTENT_ADS_INLINE_CONTENT_AD_BUILDER_H_

#include <string>

namespace hns_ads {

struct CreativeInlineContentAdInfo;
struct InlineContentAdInfo;

InlineContentAdInfo BuildInlineContentAd(
    const CreativeInlineContentAdInfo& creative_ad);

InlineContentAdInfo BuildInlineContentAd(
    const CreativeInlineContentAdInfo& creative_ad,
    const std::string& placement_id);

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_CREATIVES_INLINE_CONTENT_ADS_INLINE_CONTENT_AD_BUILDER_H_
