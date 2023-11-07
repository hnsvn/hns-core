/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_CONVERSIONS_QUEUE_QUEUE_ITEM_CONVERSION_QUEUE_ITEM_UNITTEST_UTIL_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_CONVERSIONS_QUEUE_QUEUE_ITEM_CONVERSION_QUEUE_ITEM_UNITTEST_UTIL_H_

#include "hns/components/hns_ads/core/internal/conversions/queue/queue_item/conversion_queue_item_info.h"
#include "hns/components/hns_ads/core/public/confirmation_type.h"

namespace hns_ads {

class AdType;
class ConfirmationType;
struct ConversionInfo;

ConversionQueueItemList BuildConversionQueueItemsForTesting(
    const ConversionInfo& conversion,
    size_t count);

void SaveConversionQueueItemsForTesting(
    const ConversionQueueItemList& conversion_queue_items);

void BuildAndSaveConversionQueueItemsForTesting(
    const AdType& ad_type,
    const ConfirmationType& confirmation_type,
    bool is_verifiable,
    bool should_use_random_uuids,
    int count);

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_CONVERSIONS_QUEUE_QUEUE_ITEM_CONVERSION_QUEUE_ITEM_UNITTEST_UTIL_H_
