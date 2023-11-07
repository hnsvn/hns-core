/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_USER_INTERACTION_AD_EVENTS_INLINE_CONTENT_ADS_INLINE_CONTENT_AD_EVENT_FACTORY_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_USER_INTERACTION_AD_EVENTS_INLINE_CONTENT_ADS_INLINE_CONTENT_AD_EVENT_FACTORY_H_

#include <memory>

#include "hns/components/hns_ads/core/internal/user_interaction/ad_events/ad_event_interface.h"
#include "hns/components/hns_ads/core/mojom/hns_ads.mojom-shared.h"

namespace hns_ads {

struct InlineContentAdInfo;

class InlineContentAdEventFactory final {
 public:
  static std::unique_ptr<AdEventInterface<InlineContentAdInfo>> Build(
      mojom::InlineContentAdEventType event_type);
};

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_USER_INTERACTION_AD_EVENTS_INLINE_CONTENT_ADS_INLINE_CONTENT_AD_EVENT_FACTORY_H_
