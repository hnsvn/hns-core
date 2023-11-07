/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_TARGETING_BEHAVIORAL_PURCHASE_INTENT_RESOURCE_PURCHASE_INTENT_SIGNAL_HISTORY_INFO_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_TARGETING_BEHAVIORAL_PURCHASE_INTENT_RESOURCE_PURCHASE_INTENT_SIGNAL_HISTORY_INFO_H_

#include <map>
#include <string>
#include <vector>

#include "base/time/time.h"

namespace hns_ads {

struct PurchaseIntentSignalHistoryInfo final {
  PurchaseIntentSignalHistoryInfo(base::Time created_at, int weight);

  base::Time created_at;
  int weight = 0;
};

bool operator==(const PurchaseIntentSignalHistoryInfo&,
                const PurchaseIntentSignalHistoryInfo&);
bool operator!=(const PurchaseIntentSignalHistoryInfo&,
                const PurchaseIntentSignalHistoryInfo&);

using PurchaseIntentSignalHistoryList =
    std::vector<PurchaseIntentSignalHistoryInfo>;
using PurchaseIntentSignalHistoryMap =
    std::map</*segment*/ std::string, PurchaseIntentSignalHistoryList>;

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_TARGETING_BEHAVIORAL_PURCHASE_INTENT_RESOURCE_PURCHASE_INTENT_SIGNAL_HISTORY_INFO_H_
