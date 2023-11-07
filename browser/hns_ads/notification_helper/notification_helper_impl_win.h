/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_HNS_ADS_NOTIFICATION_HELPER_NOTIFICATION_HELPER_IMPL_WIN_H_
#define HNS_BROWSER_HNS_ADS_NOTIFICATION_HELPER_NOTIFICATION_HELPER_IMPL_WIN_H_

#include <windows.ui.notifications.h>
#include <wrl/event.h>

#include "base/memory/weak_ptr.h"
#include "hns/browser/hns_ads/notification_helper/notification_helper_impl.h"

namespace hns_ads {

class NotificationHelperImplWin
    : public NotificationHelperImpl,
      public base::SupportsWeakPtr<NotificationHelperImplWin> {
 public:
  NotificationHelperImplWin(const NotificationHelperImplWin&) = delete;
  NotificationHelperImplWin& operator=(const NotificationHelperImplWin&) =
      delete;

  NotificationHelperImplWin(NotificationHelperImplWin&&) noexcept = delete;
  NotificationHelperImplWin& operator=(NotificationHelperImplWin&&) noexcept =
      delete;

  ~NotificationHelperImplWin() override;

 protected:
  friend class NotificationHelper;

  NotificationHelperImplWin();

 private:
  bool IsFocusAssistEnabled() const;

  bool IsNotificationsEnabled();

  std::wstring GetAppId() const;

  HRESULT InitializeToastNotifier();

  template <unsigned int size>
  HRESULT CreateActivationFactory(wchar_t const (&class_name)[size],
                                  const IID& iid,
                                  void** factory) const;

  Microsoft::WRL::ComPtr<ABI::Windows::UI::Notifications::IToastNotifier>
      toast_notifier_;

  // NotificationHelperImpl:
  bool CanShowNotifications() override;
  bool CanShowSystemNotificationsWhileBrowserIsBackgrounded() const override;

  bool ShowOnboardingNotification() override;
};

}  // namespace hns_ads

#endif  // HNS_BROWSER_HNS_ADS_NOTIFICATION_HELPER_NOTIFICATION_HELPER_IMPL_WIN_H_
