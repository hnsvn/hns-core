/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ADS_NOTIFICATION_AD_NOTIFICATION_AD_HANDLER_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ADS_NOTIFICATION_AD_NOTIFICATION_AD_HANDLER_H_

#include <string>

#include "base/memory/raw_ref.h"
#include "base/memory/weak_ptr.h"
#include "hns/components/hns_ads/core/internal/browser/browser_manager_observer.h"
#include "hns/components/hns_ads/core/internal/segments/segment_alias.h"
#include "hns/components/hns_ads/core/internal/serving/notification_ad_serving.h"
#include "hns/components/hns_ads/core/internal/serving/notification_ad_serving_delegate.h"
#include "hns/components/hns_ads/core/internal/user_interaction/ad_events/notification_ads/notification_ad_event_handler.h"
#include "hns/components/hns_ads/core/internal/user_interaction/ad_events/notification_ads/notification_ad_event_handler_delegate.h"
#include "hns/components/hns_ads/core/mojom/hns_ads.mojom-shared.h"
#include "hns/components/hns_ads/core/public/ads_callback.h"
#include "hns/components/hns_ads/core/public/client/ads_client_notifier_observer.h"

namespace base {
class TimeDelta;
}  // namespace base

namespace hns_ads {

class Account;
class AntiTargetingResource;
class EpsilonGreedyBanditProcessor;
class SubdivisionTargeting;
class Transfer;
struct NotificationAdInfo;

class NotificationAdHandler final : public AdsClientNotifierObserver,
                                    public BrowserManagerObserver,
                                    public NotificationAdEventHandlerDelegate,
                                    public NotificationAdServingDelegate {
 public:
  NotificationAdHandler(
      Account& account,
      Transfer& transfer,
      EpsilonGreedyBanditProcessor& epsilon_greedy_bandit_processor,
      const SubdivisionTargeting& subdivision_targeting,
      const AntiTargetingResource& anti_targeting_resource);

  NotificationAdHandler(const NotificationAdHandler&) = delete;
  NotificationAdHandler& operator=(const NotificationAdHandler&) = delete;

  NotificationAdHandler(NotificationAdHandler&&) noexcept = delete;
  NotificationAdHandler& operator=(NotificationAdHandler&&) noexcept = delete;

  ~NotificationAdHandler() override;

  void TriggerEvent(const std::string& placement_id,
                    mojom::NotificationAdEventType event_type,
                    TriggerAdEventCallback callback);

 private:
  void MaybeServeAtRegularIntervals();

  void FireServedEventCallback(TriggerAdEventCallback callback,
                               bool success,
                               const std::string& placement_id,
                               mojom::NotificationAdEventType event_type);

  // AdsClientNotifierObserver:
  void OnNotifyDidInitializeAds() override;
  void OnNotifyPrefDidChange(const std::string& path) override;
  void OnNotifyUserDidBecomeActive(base::TimeDelta idle_time,
                                   bool screen_was_locked) override;

  // BrowserManagerObserver:
  void OnBrowserDidEnterForeground() override;
  void OnBrowserDidEnterBackground() override;

  // NotificationAdServingDelegate:
  void OnOpportunityAroseToServeNotificationAd(
      const SegmentList& segments) override;
  void OnDidServeNotificationAd(const NotificationAdInfo& ad) override;

  // NotificationAdEventHandlerDelegate:
  void OnDidFireNotificationAdServedEvent(
      const NotificationAdInfo& ad) override;
  void OnDidFireNotificationAdViewedEvent(
      const NotificationAdInfo& ad) override;
  void OnDidFireNotificationAdClickedEvent(
      const NotificationAdInfo& ad) override;
  void OnDidFireNotificationAdDismissedEvent(
      const NotificationAdInfo& ad) override;
  void OnDidFireNotificationAdTimedOutEvent(
      const NotificationAdInfo& ad) override;

  const raw_ref<Account> account_;
  const raw_ref<Transfer> transfer_;
  const raw_ref<EpsilonGreedyBanditProcessor> epsilon_greedy_bandit_processor_;

  NotificationAdEventHandler event_handler_;

  NotificationAdServing serving_;

  base::WeakPtrFactory<NotificationAdHandler> weak_factory_{this};
};

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ADS_NOTIFICATION_AD_NOTIFICATION_AD_HANDLER_H_
