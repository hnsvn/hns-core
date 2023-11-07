/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_HISTORY_SORTS_HISTORY_SORT_FACTORY_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_HISTORY_SORTS_HISTORY_SORT_FACTORY_H_

#include <memory>

#include "hns/components/hns_ads/core/internal/history/sorts/history_sort_interface.h"
#include "hns/components/hns_ads/core/public/history/history_sort_types.h"

namespace hns_ads {

class HistorySortFactory final {
 public:
  static std::unique_ptr<HistorySortInterface> Build(HistorySortType type);
};

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_HISTORY_SORTS_HISTORY_SORT_FACTORY_H_
