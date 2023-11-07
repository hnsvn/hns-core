/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_TARGETING_BEHAVIORAL_PURCHASE_INTENT_RESOURCE_PURCHASE_INTENT_SIGNAL_HISTORY_VALUE_UTIL_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_TARGETING_BEHAVIORAL_PURCHASE_INTENT_RESOURCE_PURCHASE_INTENT_SIGNAL_HISTORY_VALUE_UTIL_H_

#include "base/values.h"

namespace hns_ads {

struct PurchaseIntentSignalHistoryInfo;

base::Value::Dict PurchaseIntentSignalHistoryToValue(
    const PurchaseIntentSignalHistoryInfo& purchase_intent_signal_history);

PurchaseIntentSignalHistoryInfo PurchaseIntentSignalHistoryFromValue(
    const base::Value::Dict& dict);

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_TARGETING_BEHAVIORAL_PURCHASE_INTENT_RESOURCE_PURCHASE_INTENT_SIGNAL_HISTORY_VALUE_UTIL_H_
