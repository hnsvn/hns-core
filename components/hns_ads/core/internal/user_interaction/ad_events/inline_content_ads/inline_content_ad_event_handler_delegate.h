/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_USER_INTERACTION_AD_EVENTS_INLINE_CONTENT_ADS_INLINE_CONTENT_AD_EVENT_HANDLER_DELEGATE_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_USER_INTERACTION_AD_EVENTS_INLINE_CONTENT_ADS_INLINE_CONTENT_AD_EVENT_HANDLER_DELEGATE_H_

#include <string>

#include "hns/components/hns_ads/core/mojom/hns_ads.mojom-forward.h"

namespace hns_ads {

struct InlineContentAdInfo;

class InlineContentAdEventHandlerDelegate {
 public:
  // Invoked when the inline content |ad| is served.
  virtual void OnDidFireInlineContentAdServedEvent(
      const InlineContentAdInfo& ad) {}

  // Invoked when the inline content |ad| is viewed.
  virtual void OnDidFireInlineContentAdViewedEvent(
      const InlineContentAdInfo& ad) {}

  // Invoked when the inline content |ad| is clicked.
  virtual void OnDidFireInlineContentAdClickedEvent(
      const InlineContentAdInfo& ad) {}

  // Invoked when the inline content |ad| event fails for |placement_id|,
  // |creative_instance_id| and |event_type|.
  virtual void OnFailedToFireInlineContentAdEvent(
      const std::string& placement_id,
      const std::string& creative_instance_id,
      const mojom::InlineContentAdEventType event_type) {}

 protected:
  virtual ~InlineContentAdEventHandlerDelegate() = default;
};

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_USER_INTERACTION_AD_EVENTS_INLINE_CONTENT_ADS_INLINE_CONTENT_AD_EVENT_HANDLER_DELEGATE_H_
