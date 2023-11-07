/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_HNS_ADS_NOTIFICATION_HELPER_NOTIFICATION_HELPER_H_
#define HNS_BROWSER_HNS_ADS_NOTIFICATION_HELPER_NOTIFICATION_HELPER_H_

#include <memory>

#include "base/memory/weak_ptr.h"

class Profile;

namespace base {
template <typename T>
class NoDestructor;
}  // namespace base

namespace hns_ads {

class NotificationHelperImpl;

class NotificationHelper final {
 public:
  NotificationHelper(const NotificationHelper&) = delete;
  NotificationHelper& operator=(const NotificationHelper&) = delete;

  NotificationHelper(NotificationHelper&&) noexcept = delete;
  NotificationHelper& operator=(NotificationHelper&&) noexcept = delete;

  static NotificationHelper* GetInstance();

  void InitForProfile(Profile* profile);

  bool CanShowNotifications();
  bool CanShowSystemNotificationsWhileBrowserIsBackgrounded() const;

  bool ShowOnboardingNotification();

  bool DoesSupportSystemNotifications() const;

 private:
  friend base::NoDestructor<NotificationHelper>;

  NotificationHelper();

  ~NotificationHelper();

  void OnSystemNotificationPlatformBridgeReady(bool success);

  bool does_support_system_notifications_ = true;

  std::unique_ptr<NotificationHelperImpl> impl_;

  base::WeakPtrFactory<NotificationHelper> weak_factory_{this};
};

}  // namespace hns_ads

#endif  // HNS_BROWSER_HNS_ADS_NOTIFICATION_HELPER_NOTIFICATION_HELPER_H_
