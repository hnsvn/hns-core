/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_CONVERSIONS_QUEUE_QUEUE_ITEM_CONVERSION_QUEUE_ITEM_INFO_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_CONVERSIONS_QUEUE_QUEUE_ITEM_CONVERSION_QUEUE_ITEM_INFO_H_

#include <vector>

#include "base/time/time.h"
#include "hns/components/hns_ads/core/internal/conversions/conversion/conversion_info.h"

namespace hns_ads {

struct ConversionQueueItemInfo final {
  ConversionQueueItemInfo();

  ConversionQueueItemInfo(const ConversionQueueItemInfo&);
  ConversionQueueItemInfo& operator=(const ConversionQueueItemInfo&);

  ConversionQueueItemInfo(ConversionQueueItemInfo&&) noexcept;
  ConversionQueueItemInfo& operator=(ConversionQueueItemInfo&&) noexcept;

  ~ConversionQueueItemInfo();

  [[nodiscard]] bool IsValid() const;

  ConversionInfo conversion;
  base::Time process_at;
  bool was_processed = false;
};

bool operator==(const ConversionQueueItemInfo&, const ConversionQueueItemInfo&);
bool operator!=(const ConversionQueueItemInfo&, const ConversionQueueItemInfo&);

using ConversionQueueItemList = std::vector<ConversionQueueItemInfo>;

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_CONVERSIONS_QUEUE_QUEUE_ITEM_CONVERSION_QUEUE_ITEM_INFO_H_
