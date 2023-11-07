/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "base/functional/callback_helpers.h"
#include "base/memory/raw_ptr.h"
#include "hns/browser/ui/hns_rewards/rewards_panel_coordinator.h"
#include "hns/browser/ui/views/hns_actions/hns_actions_container.h"
#include "hns/browser/ui/views/hns_actions/hns_rewards_action_view.h"
#include "hns/browser/ui/views/location_bar/hns_location_bar_view.h"
#include "hns/components/hns_rewards/common/pref_names.h"
#include "hns/components/constants/pref_names.h"
#include "chrome/browser/browser_process.h"
#include "chrome/browser/chrome_notification_types.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/profiles/profile_manager.h"
#include "chrome/browser/profiles/profile_window.h"
#include "chrome/browser/search_engines/template_url_service_factory.h"
#include "chrome/browser/ui/browser.h"
#include "chrome/browser/ui/browser_finder.h"
#include "chrome/browser/ui/browser_list.h"
#include "chrome/browser/ui/views/frame/browser_view.h"
#include "chrome/test/base/in_process_browser_test.h"
#include "chrome/test/base/search_test_utils.h"
#include "chrome/test/base/ui_test_utils.h"
#include "components/prefs/pref_service.h"
#include "content/public/browser/notification_service.h"
#include "content/public/test/browser_test.h"
#include "content/public/test/test_utils.h"

class HnsActionsContainerTest : public InProcessBrowserTest {
 public:
  HnsActionsContainerTest() = default;
  HnsActionsContainerTest(const HnsActionsContainerTest&) = delete;
  HnsActionsContainerTest& operator=(const HnsActionsContainerTest&) =
      delete;
  ~HnsActionsContainerTest() override = default;

  void SetUpOnMainThread() override { Init(browser()); }

  void Init(Browser* browser) {
    BrowserView* browser_view = BrowserView::GetBrowserViewForBrowser(browser);
    ASSERT_NE(browser_view, nullptr);
    HnsLocationBarView* hns_location_bar_view =
        static_cast<HnsLocationBarView*>(browser_view->GetLocationBarView());
    ASSERT_NE(hns_location_bar_view, nullptr);
    hns_actions_ = hns_location_bar_view->hns_actions_;
    ASSERT_NE(hns_actions_, nullptr);
    prefs_ = browser->profile()->GetPrefs();
  }

  void CheckHnsRewardsActionShown(bool expected_shown) {
    const bool shown = hns_actions_->rewards_action_btn_->GetVisible();
    ASSERT_EQ(shown, expected_shown);
  }

  void CloseRewardsPanel() {
    hns_actions_->rewards_action_btn_->ClosePanelForTesting();
  }

 protected:
  raw_ptr<HnsActionsContainer> hns_actions_ = nullptr;
  raw_ptr<PrefService> prefs_ = nullptr;
};

IN_PROC_BROWSER_TEST_F(HnsActionsContainerTest, HideHnsRewardsAction) {
  // By default the action should be shown.
  EXPECT_TRUE(prefs_->GetBoolean(hns_rewards::prefs::kShowLocationBarButton));
  CheckHnsRewardsActionShown(true);

  // Set to hide.
  prefs_->SetBoolean(hns_rewards::prefs::kShowLocationBarButton, false);
  CheckHnsRewardsActionShown(false);

  // Set to show.
  prefs_->SetBoolean(hns_rewards::prefs::kShowLocationBarButton, true);
  CheckHnsRewardsActionShown(true);
}

IN_PROC_BROWSER_TEST_F(HnsActionsContainerTest,
                       HnsRewardsActionHiddenInGuestSession) {
  // By default the action should be shown.
  EXPECT_TRUE(prefs_->GetBoolean(hns_rewards::prefs::kShowLocationBarButton));
  CheckHnsRewardsActionShown(true);

  // Open a Guest window.
  EXPECT_EQ(1U, BrowserList::GetInstance()->size());
  ui_test_utils::BrowserChangeObserver browser_creation_observer(
      nullptr, ui_test_utils::BrowserChangeObserver::ChangeType::kAdded);
  profiles::SwitchToGuestProfile(base::DoNothing());
  base::RunLoop().RunUntilIdle();
  browser_creation_observer.Wait();
  EXPECT_EQ(2U, BrowserList::GetInstance()->size());

  // Retrieve the new Guest profile.
  Profile* guest = g_browser_process->profile_manager()->GetProfileByPath(
      ProfileManager::GetGuestProfilePath());
  // The BrowsingDataRemover needs a loaded TemplateUrlService or else it hangs
  // on to a CallbackList::Subscription forever.
  search_test_utils::WaitForTemplateURLServiceToLoad(
      TemplateURLServiceFactory::GetForProfile(guest));

  // Access the browser with the Guest profile and re-init test for it.
  Browser* browser = chrome::FindAnyBrowser(guest, true);
  EXPECT_TRUE(browser);
  Init(browser);
  CheckHnsRewardsActionShown(false);
}

IN_PROC_BROWSER_TEST_F(HnsActionsContainerTest, ShowRewardsIconForPanel) {
  prefs_->SetBoolean(hns_rewards::prefs::kShowLocationBarButton, false);
  CheckHnsRewardsActionShown(false);

  // Send a request to open the Rewards panel.
  auto* coordinator =
      hns_rewards::RewardsPanelCoordinator::FromBrowser(browser());

  ASSERT_TRUE(coordinator);
  coordinator->OpenRewardsPanel();
  base::RunLoop().RunUntilIdle();

  CheckHnsRewardsActionShown(false);
}
