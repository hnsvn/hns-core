/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_USER_ATTENTION_USER_ACTIVITY_USER_ACTIVITY_MANAGER_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_USER_ATTENTION_USER_ACTIVITY_USER_ACTIVITY_MANAGER_H_

#include <cstdint>

#include "hns/components/hns_ads/core/internal/browser/browser_manager_observer.h"
#include "hns/components/hns_ads/core/internal/tabs/tab_manager_observer.h"
#include "hns/components/hns_ads/core/internal/user_attention/user_activity/user_activity_event_info.h"
#include "hns/components/hns_ads/core/internal/user_attention/user_activity/user_activity_event_types.h"
#include "hns/components/hns_ads/core/public/client/ads_client_notifier_observer.h"
#include "hns/components/hns_ads/core/public/user_attention/page_transition_types.h"

namespace base {
class TimeDelta;
}  // namespace base

namespace hns_ads {

struct TabInfo;

class UserActivityManager final : public AdsClientNotifierObserver,
                                  public BrowserManagerObserver,
                                  public TabManagerObserver {
 public:
  UserActivityManager();

  UserActivityManager(const UserActivityManager&) = delete;
  UserActivityManager& operator=(const UserActivityManager&) = delete;

  UserActivityManager(UserActivityManager&&) noexcept = delete;
  UserActivityManager& operator=(UserActivityManager&&) noexcept = delete;

  ~UserActivityManager() override;

  static UserActivityManager& GetInstance();

  void RecordEvent(UserActivityEventType event_type);

  UserActivityEventList GetHistoryForTimeWindow(
      base::TimeDelta time_window) const;

 private:
  void RecordEventForPageTransition(PageTransitionType type);

  // AdsClientNotifierObserver:
  void OnNotifyDidInitializeAds() override;
  void OnNotifyUserGestureEventTriggered(int32_t type) override;

  // BrowserManagerObserver:
  void OnBrowserDidBecomeActive() override;
  void OnBrowserDidResignActive() override;
  void OnBrowserDidEnterForeground() override;
  void OnBrowserDidEnterBackground() override;

  // TabManagerObserver:
  void OnTabDidChangeFocus(int32_t tab_id) override;
  void OnTabDidChange(const TabInfo& tab) override;
  void OnDidOpenNewTab(const TabInfo& tab) override;
  void OnDidCloseTab(int32_t tab_id) override;
  void OnTabDidStartPlayingMedia(int32_t tab_id) override;
  void OnTabDidStopPlayingMedia(int32_t tab_id) override;

  UserActivityEventList history_;
};

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_USER_ATTENTION_USER_ACTIVITY_USER_ACTIVITY_MANAGER_H_
