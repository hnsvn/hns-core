/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/history/sorts/descending_history_sort.h"

#include "base/ranges/algorithm.h"
#include "base/ranges/functional.h"
#include "hns/components/hns_ads/core/public/history/history_item_info.h"

namespace hns_ads {

HistoryItemList DescendingHistorySort::Apply(
    const HistoryItemList& history) const {
  HistoryItemList sorted_history = history;

  base::ranges::sort(sorted_history, base::ranges::greater{},
                     &HistoryItemInfo::created_at);

  return sorted_history;
}

}  // namespace hns_ads
