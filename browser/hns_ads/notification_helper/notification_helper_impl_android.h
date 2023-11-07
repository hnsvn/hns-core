/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_HNS_ADS_NOTIFICATION_HELPER_NOTIFICATION_HELPER_IMPL_ANDROID_H_
#define HNS_BROWSER_HNS_ADS_NOTIFICATION_HELPER_NOTIFICATION_HELPER_IMPL_ANDROID_H_

#include "base/memory/weak_ptr.h"
#include "hns/browser/hns_ads/notification_helper/notification_helper_impl.h"

namespace hns_ads {

class NotificationHelperImplAndroid
    : public NotificationHelperImpl,
      public base::SupportsWeakPtr<NotificationHelperImplAndroid> {
 public:
  NotificationHelperImplAndroid(const NotificationHelperImplAndroid&) = delete;
  NotificationHelperImplAndroid& operator=(
      const NotificationHelperImplAndroid&) = delete;

  NotificationHelperImplAndroid(NotificationHelperImplAndroid&&) noexcept =
      delete;
  NotificationHelperImplAndroid& operator=(
      NotificationHelperImplAndroid&&) noexcept = delete;

  ~NotificationHelperImplAndroid() override;

 protected:
  friend class NotificationHelper;

  NotificationHelperImplAndroid();

 private:
  // NotificationHelperLinux:
  bool CanShowNotifications() override;
  bool CanShowSystemNotificationsWhileBrowserIsBackgrounded() const override;

  bool ShowOnboardingNotification() override;
};

}  // namespace hns_ads

#endif  // HNS_BROWSER_HNS_ADS_NOTIFICATION_HELPER_NOTIFICATION_HELPER_IMPL_ANDROID_H_
