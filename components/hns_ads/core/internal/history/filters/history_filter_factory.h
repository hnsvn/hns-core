/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_HISTORY_FILTERS_HISTORY_FILTER_FACTORY_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_HISTORY_FILTERS_HISTORY_FILTER_FACTORY_H_

#include <memory>

#include "hns/components/hns_ads/core/internal/history/filters/history_filter_interface.h"
#include "hns/components/hns_ads/core/public/history/history_filter_types.h"

namespace hns_ads {

class HistoryFilterFactory final {
 public:
  static std::unique_ptr<HistoryFilterInterface> Build(HistoryFilterType type);
};

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_HISTORY_FILTERS_HISTORY_FILTER_FACTORY_H_
