/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/conversions/queue/queue_item/conversion_queue_item_unittest_util.h"

#include "base/check.h"
#include "base/functional/bind.h"
#include "hns/components/hns_ads/core/internal/ads/ad_unittest_util.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_time_util.h"
#include "hns/components/hns_ads/core/internal/conversions/conversion/conversion_builder.h"
#include "hns/components/hns_ads/core/internal/conversions/conversion/conversion_info.h"
#include "hns/components/hns_ads/core/internal/conversions/queue/conversion_queue_database_table.h"
#include "hns/components/hns_ads/core/internal/conversions/queue/queue_item/conversion_queue_item_builder.h"
#include "hns/components/hns_ads/core/internal/conversions/types/verifiable_conversion/verifiable_conversion_info.h"
#include "hns/components/hns_ads/core/internal/conversions/types/verifiable_conversion/verifiable_conversion_unittest_constants.h"
#include "hns/components/hns_ads/core/internal/user_interaction/ad_events/ad_event_builder.h"
#include "hns/components/hns_ads/core/internal/user_interaction/ad_events/ad_event_info.h"
#include "hns/components/hns_ads/core/public/ad_info.h"
#include "hns/components/hns_ads/core/public/confirmation_type.h"

namespace hns_ads {

ConversionQueueItemList BuildConversionQueueItemsForTesting(
    const ConversionInfo& conversion,
    const size_t count) {
  ConversionQueueItemList conversion_queue_items;

  for (size_t i = 0; i < count; i++) {
    const ConversionQueueItemInfo conversion_queue_item =
        BuildConversionQueueItem(conversion, /*process_at*/ Now());

    conversion_queue_items.push_back(conversion_queue_item);
  }

  return conversion_queue_items;
}

void SaveConversionQueueItemsForTesting(
    const ConversionQueueItemList& conversion_queue_items) {
  const database::table::ConversionQueue database_table;
  database_table.Save(
      conversion_queue_items,
      base::BindOnce([](const bool success) { CHECK(success); }));
}

void BuildAndSaveConversionQueueItemsForTesting(
    const AdType& ad_type,
    const ConfirmationType& confirmation_type,
    const bool is_verifiable,
    const bool should_use_random_uuids,
    const int count) {
  const AdInfo ad = BuildAdForTesting(ad_type, should_use_random_uuids);

  absl::optional<VerifiableConversionInfo> verifiable_conversion;
  if (is_verifiable) {
    verifiable_conversion = VerifiableConversionInfo{
        kVerifiableConversionId, kVerifiableConversionAdvertiserPublicKey};
  }

  const ConversionInfo conversion =
      BuildConversion(BuildAdEvent(ad, confirmation_type, /*created_at*/ Now()),
                      verifiable_conversion);

  const ConversionQueueItemList conversion_queue_items =
      BuildConversionQueueItemsForTesting(conversion, count);

  SaveConversionQueueItemsForTesting(conversion_queue_items);
}

}  // namespace hns_ads
