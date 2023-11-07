/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_UI_VIEWS_HNS_ADS_NOTIFICATION_AD_POPUP_COLLECTION_H_
#define HNS_BROWSER_UI_VIEWS_HNS_ADS_NOTIFICATION_AD_POPUP_COLLECTION_H_

#include <string>

namespace hns_ads {

class NotificationAdPopup;

class NotificationAdPopupCollection final {
 public:
  NotificationAdPopupCollection();

  NotificationAdPopupCollection(const NotificationAdPopupCollection&) = delete;
  NotificationAdPopupCollection& operator=(
      const NotificationAdPopupCollection&) = delete;

  NotificationAdPopupCollection(NotificationAdPopupCollection&&) noexcept =
      delete;
  NotificationAdPopupCollection& operator=(
      NotificationAdPopupCollection&&) noexcept = delete;

  ~NotificationAdPopupCollection();

  static void Add(NotificationAdPopup* popup,
                  const std::string& notification_id);
  static NotificationAdPopup* Get(const std::string& notification_id);
  static void Remove(const std::string& notification_id);
};

}  // namespace hns_ads

#endif  // HNS_BROWSER_UI_VIEWS_HNS_ADS_NOTIFICATION_AD_POPUP_COLLECTION_H_
