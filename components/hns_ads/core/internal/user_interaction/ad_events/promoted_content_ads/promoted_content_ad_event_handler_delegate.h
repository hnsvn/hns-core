/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_USER_INTERACTION_AD_EVENTS_PROMOTED_CONTENT_ADS_PROMOTED_CONTENT_AD_EVENT_HANDLER_DELEGATE_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_USER_INTERACTION_AD_EVENTS_PROMOTED_CONTENT_ADS_PROMOTED_CONTENT_AD_EVENT_HANDLER_DELEGATE_H_

#include <string>

#include "hns/components/hns_ads/core/mojom/hns_ads.mojom-shared.h"

namespace hns_ads {

struct PromotedContentAdInfo;

class PromotedContentAdEventHandlerDelegate {
 public:
  // Invoked when the promoted content |ad| is served.
  virtual void OnDidFirePromotedContentAdServedEvent(
      const PromotedContentAdInfo& ad) {}

  // Invoked when the promoted content |ad| is viewed.
  virtual void OnDidFirePromotedContentAdViewedEvent(
      const PromotedContentAdInfo& ad) {}

  // Invoked when the promoted content |ad| is clicked.
  virtual void OnDidFirePromotedContentAdClickedEvent(
      const PromotedContentAdInfo& ad) {}

  // Invoked when the promoted content |ad| event fails for |placement_id|,
  // |creative_instance_id| and |event_type|.
  virtual void OnFailedToFirePromotedContentAdEvent(
      const std::string& placement_id,
      const std::string& creative_instance_id,
      const mojom::PromotedContentAdEventType event_type) {}

 protected:
  virtual ~PromotedContentAdEventHandlerDelegate() = default;
};

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_USER_INTERACTION_AD_EVENTS_PROMOTED_CONTENT_ADS_PROMOTED_CONTENT_AD_EVENT_HANDLER_DELEGATE_H_
