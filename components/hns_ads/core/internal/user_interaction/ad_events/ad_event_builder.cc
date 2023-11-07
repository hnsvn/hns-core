/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/user_interaction/ad_events/ad_event_builder.h"

#include "base/time/time.h"
#include "hns/components/hns_ads/core/internal/user_interaction/ad_events/ad_event_info.h"
#include "hns/components/hns_ads/core/public/ad_info.h"
#include "hns/components/hns_ads/core/public/confirmation_type.h"

namespace hns_ads {

AdEventInfo BuildAdEvent(const AdInfo& ad,
                         const ConfirmationType& confirmation_type,
                         const base::Time created_at) {
  AdEventInfo ad_event;

  ad_event.type = ad.type;
  ad_event.confirmation_type = confirmation_type;
  ad_event.placement_id = ad.placement_id;
  ad_event.campaign_id = ad.campaign_id;
  ad_event.creative_set_id = ad.creative_set_id;
  ad_event.creative_instance_id = ad.creative_instance_id;
  ad_event.advertiser_id = ad.advertiser_id;
  ad_event.segment = ad.segment;
  ad_event.created_at = created_at;

  return ad_event;
}

AdEventInfo RebuildAdEvent(const AdEventInfo& ad_event,
                           const ConfirmationType& confirmation_type,
                           const base::Time created_at) {
  AdEventInfo mutable_ad_event(ad_event);

  mutable_ad_event.confirmation_type = confirmation_type;
  mutable_ad_event.created_at = created_at;

  return mutable_ad_event;
}

}  // namespace hns_ads
