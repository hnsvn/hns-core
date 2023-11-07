/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_CREATIVES_CONVERSIONS_CREATIVE_SET_CONVERSION_BUILDER_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_CREATIVES_CONVERSIONS_CREATIVE_SET_CONVERSION_BUILDER_H_

#include "hns/components/hns_ads/core/mojom/hns_ads.mojom-forward.h"
#include "third_party/abseil-cpp/absl/types/optional.h"

namespace hns_ads {

struct CreativeSetConversionInfo;

absl::optional<CreativeSetConversionInfo> BuildCreativeSetConversion(
    const mojom::SearchResultAdInfoPtr& ad_mojom);

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_CREATIVES_CONVERSIONS_CREATIVE_SET_CONVERSION_BUILDER_H_
