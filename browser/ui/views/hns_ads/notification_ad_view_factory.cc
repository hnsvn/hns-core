/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/browser/ui/views/hns_ads/notification_ad_view_factory.h"

#include "hns/browser/ui/views/hns_ads/text_notification_ad_view.h"

namespace hns_ads {

// static
std::unique_ptr<NotificationAdView> NotificationAdViewFactory::Create(
    const NotificationAd& notification_ad) {
  return std::make_unique<TextNotificationAdView>(notification_ad);
}

}  // namespace hns_ads
