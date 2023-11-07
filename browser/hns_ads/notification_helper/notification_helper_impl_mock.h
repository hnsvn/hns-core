/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_HNS_ADS_NOTIFICATION_HELPER_NOTIFICATION_HELPER_IMPL_MOCK_H_
#define HNS_BROWSER_HNS_ADS_NOTIFICATION_HELPER_NOTIFICATION_HELPER_IMPL_MOCK_H_

#include "hns/browser/hns_ads/notification_helper/notification_helper_impl.h"
#include "testing/gmock/include/gmock/gmock.h"

namespace hns_ads {

class NotificationHelperImplMock : public NotificationHelperImpl {
 public:
  NotificationHelperImplMock();

  NotificationHelperImplMock(const NotificationHelperImplMock&) = delete;
  NotificationHelperImplMock& operator=(const NotificationHelperImplMock&) =
      delete;

  NotificationHelperImplMock(NotificationHelperImplMock&&) noexcept = delete;
  NotificationHelperImplMock& operator=(NotificationHelperImplMock&&) noexcept =
      delete;

  ~NotificationHelperImplMock() override;

  MOCK_METHOD0(CanShowNotifications, bool());
  MOCK_CONST_METHOD0(CanShowSystemNotificationsWhileBrowserIsBackgrounded,
                     bool());

  MOCK_METHOD0(ShowOnboardingNotification, bool());
};

}  // namespace hns_ads

#endif  // HNS_BROWSER_HNS_ADS_NOTIFICATION_HELPER_NOTIFICATION_HELPER_IMPL_MOCK_H_
