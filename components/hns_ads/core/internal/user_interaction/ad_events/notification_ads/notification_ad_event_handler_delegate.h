/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_USER_INTERACTION_AD_EVENTS_NOTIFICATION_ADS_NOTIFICATION_AD_EVENT_HANDLER_DELEGATE_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_USER_INTERACTION_AD_EVENTS_NOTIFICATION_ADS_NOTIFICATION_AD_EVENT_HANDLER_DELEGATE_H_

#include <string>

#include "hns/components/hns_ads/core/mojom/hns_ads.mojom-shared.h"

namespace hns_ads {

struct NotificationAdInfo;

class NotificationAdEventHandlerDelegate {
 public:
  // Invoked when the notification |ad| is served.
  virtual void OnDidFireNotificationAdServedEvent(
      const NotificationAdInfo& ad) {}

  // Invoked when the notification |ad| is viewed.
  virtual void OnDidFireNotificationAdViewedEvent(
      const NotificationAdInfo& ad) {}

  // Invoked when the notification |ad| is clicked.
  virtual void OnDidFireNotificationAdClickedEvent(
      const NotificationAdInfo& ad) {}

  // Invoked when the notification |ad| is dismissed.
  virtual void OnDidFireNotificationAdDismissedEvent(
      const NotificationAdInfo& ad) {}

  // Invoked when the notification |ad| times out.
  virtual void OnDidFireNotificationAdTimedOutEvent(
      const NotificationAdInfo& ad) {}

  // Invoked when the notification |ad| event fails for |placement_id| and
  // |event_type|.
  virtual void OnFailedToFireNotificationAdEvent(
      const std::string& placement_id,
      const mojom::NotificationAdEventType event_type) {}

 protected:
  virtual ~NotificationAdEventHandlerDelegate() = default;
};

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_USER_INTERACTION_AD_EVENTS_NOTIFICATION_ADS_NOTIFICATION_AD_EVENT_HANDLER_DELEGATE_H_
