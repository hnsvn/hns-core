/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_HISTORY_SORTS_DESCENDING_HISTORY_SORT_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_HISTORY_SORTS_DESCENDING_HISTORY_SORT_H_

#include "hns/components/hns_ads/core/internal/history/sorts/history_sort_interface.h"

namespace hns_ads {

class DescendingHistorySort final : public HistorySortInterface {
 public:
  HistoryItemList Apply(const HistoryItemList& history) const override;
};

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_HISTORY_SORTS_DESCENDING_HISTORY_SORT_H_
