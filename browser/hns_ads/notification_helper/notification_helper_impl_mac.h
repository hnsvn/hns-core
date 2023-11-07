/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_HNS_ADS_NOTIFICATION_HELPER_NOTIFICATION_HELPER_IMPL_MAC_H_
#define HNS_BROWSER_HNS_ADS_NOTIFICATION_HELPER_NOTIFICATION_HELPER_IMPL_MAC_H_

#include "base/memory/weak_ptr.h"
#include "hns/browser/hns_ads/notification_helper/notification_helper_impl.h"

namespace hns_ads {

class NotificationHelperImplMac
    : public NotificationHelperImpl,
      public base::SupportsWeakPtr<NotificationHelperImplMac> {
 public:
  NotificationHelperImplMac(const NotificationHelperImplMac&) = delete;
  NotificationHelperImplMac& operator=(const NotificationHelperImplMac&) =
      delete;

  NotificationHelperImplMac(NotificationHelperImplMac&&) noexcept = delete;
  NotificationHelperImplMac& operator=(NotificationHelperImplMac&&) noexcept =
      delete;

  ~NotificationHelperImplMac() override;

 protected:
  friend class NotificationHelper;

  NotificationHelperImplMac();

 private:
  // NotificationHelperImpl:
  bool CanShowNotifications() override;
  bool CanShowSystemNotificationsWhileBrowserIsBackgrounded() const override;

  bool ShowOnboardingNotification() override;
};

}  // namespace hns_ads

#endif  // HNS_BROWSER_HNS_ADS_NOTIFICATION_HELPER_NOTIFICATION_HELPER_IMPL_MAC_H_
