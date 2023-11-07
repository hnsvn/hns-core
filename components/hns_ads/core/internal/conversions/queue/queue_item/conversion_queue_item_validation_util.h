/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_CONVERSIONS_QUEUE_QUEUE_ITEM_CONVERSION_QUEUE_ITEM_VALIDATION_UTIL_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_CONVERSIONS_QUEUE_QUEUE_ITEM_CONVERSION_QUEUE_ITEM_VALIDATION_UTIL_H_

#include <string>

namespace hns_ads {

struct ConversionQueueItemInfo;

std::string GetConversionQueueItemInvalidFieldsNames(
    const ConversionQueueItemInfo& conversion_queue_item);

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_CONVERSIONS_QUEUE_QUEUE_ITEM_CONVERSION_QUEUE_ITEM_VALIDATION_UTIL_H_
