/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_HNS_ADS_NOTIFICATIONS_NOTIFICATION_AD_PLATFORM_BRIDGE_H_
#define HNS_BROWSER_HNS_ADS_NOTIFICATIONS_NOTIFICATION_AD_PLATFORM_BRIDGE_H_

#include <string>

#include "base/memory/raw_ref.h"
#include "hns/browser/ui/hns_ads/notification_ad.h"
#include "chrome/browser/profiles/profile.h"

namespace hns_ads {

class NotificationAdPlatformBridge {
 public:
  explicit NotificationAdPlatformBridge(Profile& profile);

  NotificationAdPlatformBridge(const NotificationAdPlatformBridge&) = delete;
  NotificationAdPlatformBridge& operator=(const NotificationAdPlatformBridge&) =
      delete;

  NotificationAdPlatformBridge(NotificationAdPlatformBridge&&) noexcept =
      delete;
  NotificationAdPlatformBridge& operator=(
      NotificationAdPlatformBridge&&) noexcept = delete;

  ~NotificationAdPlatformBridge();

  void ShowNotificationAd(NotificationAd notification_ad);
  void CloseNotificationAd(const std::string& notification_id);

 private:
  raw_ref<Profile> profile_;
};

}  // namespace hns_ads

#endif  // HNS_BROWSER_HNS_ADS_NOTIFICATIONS_NOTIFICATION_AD_PLATFORM_BRIDGE_H_
