// Copyright (c) 2022 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef HNS_BROWSER_UI_VIEWS_HNS_ACTIONS_HNS_REWARDS_ACTION_VIEW_H_
#define HNS_BROWSER_UI_VIEWS_HNS_ACTIONS_HNS_REWARDS_ACTION_VIEW_H_

#include <memory>
#include <string>
#include <utility>

#include "base/memory/raw_ptr.h"
#include "base/memory/weak_ptr.h"
#include "hns/browser/hns_rewards/rewards_tab_helper.h"
#include "hns/browser/ui/hns_rewards/rewards_panel_coordinator.h"
#include "hns/components/hns_rewards/browser/rewards_notification_service.h"
#include "hns/components/hns_rewards/browser/rewards_notification_service_observer.h"
#include "hns/components/hns_rewards/browser/rewards_service.h"
#include "hns/components/hns_rewards/browser/rewards_service_observer.h"
#include "chrome/browser/ui/tabs/tab_strip_model_observer.h"
#include "chrome/browser/ui/views/toolbar/toolbar_button.h"
#include "components/prefs/pref_change_registrar.h"
#include "ui/gfx/geometry/skia_conversions.h"
#include "ui/views/widget/widget_observer.h"

class Profile;
class TabStripModel;
class WebUIBubbleManager;

// A button that lives in the actions container and opens the Rewards panel. The
// button has an associated context menu and can be hidden by user settings.
class HnsRewardsActionView
    : public ToolbarButton,
      public views::WidgetObserver,
      public TabStripModelObserver,
      public hns_rewards::RewardsTabHelper::Observer,
      public hns_rewards::RewardsPanelCoordinator::Observer,
      public hns_rewards::RewardsServiceObserver,
      public hns_rewards::RewardsNotificationServiceObserver {
 public:
  explicit HnsRewardsActionView(Browser* browser);

  ~HnsRewardsActionView() override;

  HnsRewardsActionView(const HnsRewardsActionView&) = delete;
  HnsRewardsActionView& operator=(const HnsRewardsActionView&) = delete;

  void Update();

  void ClosePanelForTesting();

  // views::View:
  gfx::Rect GetAnchorBoundsInScreen() const override;

  // views::LabelButton:
  std::unique_ptr<views::LabelButtonBorder> CreateDefaultBorder()
      const override;

  // views::WidgetObserver:
  void OnWidgetDestroying(views::Widget* widget) override;

  // TabStripModelObserver:
  void OnTabStripModelChanged(
      TabStripModel* tab_strip_model,
      const TabStripModelChange& change,
      const TabStripSelectionChange& selection) override;

  // hns_rewards::RewardsTabHelper::Observer:
  void OnPublisherForTabUpdated(const std::string& publisher_id) override;

  // hns_rewards::RewardsPanelCoordinator::Observer:
  void OnRewardsPanelRequested(
      const hns_rewards::mojom::RewardsPanelArgs& args) override;

  // hns_rewards::RewardsServiceObserver:
  void OnPublisherRegistryUpdated() override;

  void OnPublisherUpdated(const std::string& publisher_id) override;

  // hns_rewards::RewardsNotificationServiceObserver:
  void OnNotificationAdded(
      hns_rewards::RewardsNotificationService* service,
      const hns_rewards::RewardsNotificationService::RewardsNotification&
          notification) override;

  void OnNotificationDeleted(
      hns_rewards::RewardsNotificationService* service,
      const hns_rewards::RewardsNotificationService::RewardsNotification&
          notification) override;

 private:
  void OnButtonPressed();
  void OnPreferencesChanged(const std::string& key);
  content::WebContents* GetActiveWebContents();
  hns_rewards::RewardsService* GetRewardsService();
  hns_rewards::RewardsNotificationService* GetNotificationService();
  bool IsPanelOpen();
  void ToggleRewardsPanel();
  gfx::ImageSkia GetRewardsIcon();
  std::pair<std::string, SkColor> GetBadgeTextAndBackground();
  size_t GetRewardsNotificationCount();
  bool UpdatePublisherStatus();
  void IsPublisherRegisteredCallback(const std::string& publisher_id,
                                     bool is_registered);
  void UpdateTabHelper(content::WebContents* web_contents);

  using WidgetObservation =
      base::ScopedObservation<views::Widget, views::WidgetObserver>;

  using RewardsObservation =
      base::ScopedObservation<hns_rewards::RewardsService,
                              hns_rewards::RewardsServiceObserver>;

  using NotificationServiceObservation = base::ScopedObservation<
      hns_rewards::RewardsNotificationService,
      hns_rewards::RewardsNotificationServiceObserver>;

  raw_ptr<Browser> browser_ = nullptr;
  raw_ptr<hns_rewards::RewardsPanelCoordinator> panel_coordinator_ = nullptr;
  raw_ptr<hns_rewards::RewardsTabHelper> tab_helper_ = nullptr;
  std::unique_ptr<WebUIBubbleManager> bubble_manager_;
  PrefChangeRegistrar pref_change_registrar_;
  std::pair<std::string, bool> publisher_registered_;
  hns_rewards::RewardsTabHelper::Observation tab_helper_observation_{this};
  hns_rewards::RewardsPanelCoordinator::Observation panel_observation_{this};
  WidgetObservation bubble_observation_{this};
  RewardsObservation rewards_service_observation_{this};
  NotificationServiceObservation notification_service_observation_{this};
  base::WeakPtrFactory<HnsRewardsActionView> weak_factory_{this};
};

#endif  // HNS_BROWSER_UI_VIEWS_HNS_ACTIONS_HNS_REWARDS_ACTION_VIEW_H_
