/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_HNS_REWARDS_EXTENSION_REWARDS_NOTIFICATION_SERVICE_OBSERVER_H_
#define HNS_BROWSER_HNS_REWARDS_EXTENSION_REWARDS_NOTIFICATION_SERVICE_OBSERVER_H_

#include "base/memory/raw_ptr.h"
#include "hns/components/hns_rewards/browser/rewards_notification_service_observer.h"
#include "hns/components/hns_rewards/core/mojom_structs.h"

class Profile;

namespace hns_rewards {

class RewardsNotificationService;

class ExtensionRewardsNotificationServiceObserver
    : public RewardsNotificationServiceObserver {
 public:
  explicit ExtensionRewardsNotificationServiceObserver(Profile* profile);
  ExtensionRewardsNotificationServiceObserver(
      const ExtensionRewardsNotificationServiceObserver&) = delete;
  ExtensionRewardsNotificationServiceObserver& operator=(
      const ExtensionRewardsNotificationServiceObserver&) = delete;
  ~ExtensionRewardsNotificationServiceObserver() override;

  // RewardsNotificationServiceObserver implementation
  void OnNotificationAdded(
      RewardsNotificationService* rewards_notification_service,
      const RewardsNotificationService::RewardsNotification&
          rewards_notification) override;
  void OnNotificationDeleted(
      RewardsNotificationService* rewards_notification_service,
      const RewardsNotificationService::RewardsNotification&
          rewards_notification) override;
  void OnAllNotificationsDeleted(
      RewardsNotificationService* rewards_notification_service) override;
  void OnGetNotification(
      RewardsNotificationService* rewards_notification_service,
      const RewardsNotificationService::RewardsNotification&
          rewards_notification) override;
  void OnGetAllNotifications(
      RewardsNotificationService* rewards_notification_service,
      const RewardsNotificationService::RewardsNotificationsList&
          rewards_notifications_list) override;

 private:
  raw_ptr<Profile> profile_ = nullptr;
};

}  // namespace hns_rewards

#endif  // HNS_BROWSER_HNS_REWARDS_EXTENSION_REWARDS_NOTIFICATION_SERVICE_OBSERVER_H_
