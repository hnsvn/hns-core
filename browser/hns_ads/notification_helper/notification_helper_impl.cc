/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/browser/hns_ads/notification_helper/notification_helper_impl.h"

namespace hns_ads {

NotificationHelperImpl::NotificationHelperImpl() = default;

NotificationHelperImpl::~NotificationHelperImpl() = default;

bool NotificationHelperImpl::CanShowNotifications() {
  return true;
}

bool NotificationHelperImpl::
    CanShowSystemNotificationsWhileBrowserIsBackgrounded() const {
  return true;
}

bool NotificationHelperImpl::ShowOnboardingNotification() {
  return false;
}

}  // namespace hns_ads
