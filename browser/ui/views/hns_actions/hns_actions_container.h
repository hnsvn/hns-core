/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_UI_VIEWS_HNS_ACTIONS_HNS_ACTIONS_CONTAINER_H_
#define HNS_BROWSER_UI_VIEWS_HNS_ACTIONS_HNS_ACTIONS_CONTAINER_H_

#include <memory>

#include "base/gtest_prod_util.h"
#include "base/memory/raw_ptr.h"
#include "chrome/browser/ui/browser.h"
#include "components/prefs/pref_member.h"
#include "ui/gfx/skia_util.h"
#include "ui/views/view.h"

class HnsActionViewController;
class HnsActionsContainerTest;
class HnsRewardsActionView;
class HnsShieldsActionView;
class RewardsBrowserTest;

namespace policy {
FORWARD_DECLARE_TEST(HnsRewardsPolicyTest, RewardsIconIsHidden);
}

namespace views {
class Button;
}

// This View contains all the built-in HnsActions such as Shields and Payments
// TODO(petemill): consider splitting to separate model, like
// ToolbarActionsModel and ToolbarActionsBar
class HnsActionsContainer : public views::View {
 public:
  HnsActionsContainer(Browser* browser, Profile* profile);
  HnsActionsContainer(const HnsActionsContainer&) = delete;

  HnsActionsContainer& operator=(const HnsActionsContainer&) = delete;

  ~HnsActionsContainer() override;

  void Init();
  void Update();
  void SetShouldHide(bool should_hide);

  // views::View:
  void ChildPreferredSizeChanged(views::View* child) override;

  HnsShieldsActionView* GetShieldsActionView() { return shields_action_btn_; }

 private:
  FRIEND_TEST_ALL_PREFIXES(policy::HnsRewardsPolicyTest, RewardsIconIsHidden);
  friend class ::HnsActionsContainerTest;
  friend class ::RewardsBrowserTest;

  bool ShouldShowHnsRewardsAction() const;
  void AddActionViewForRewards();
  void AddActionViewForShields();

  void UpdateVisibility();

  // Hns Rewards preferences change observers callback.
  void OnHnsRewardsPreferencesChanged();

  bool should_hide_ = false;

  // The Browser this LocationBarView is in.  Note that at least
  // chromeos::SimpleWebViewDialog uses a LocationBarView outside any browser
  // window, so this may be NULL.
  raw_ptr<Browser> browser_ = nullptr;

  raw_ptr<HnsShieldsActionView> shields_action_btn_ = nullptr;
  raw_ptr<HnsRewardsActionView> rewards_action_btn_ = nullptr;

  // Listen for Hns Rewards preferences changes.
  BooleanPrefMember show_hns_rewards_button_;

  base::WeakPtrFactory<HnsActionsContainer> weak_ptr_factory_{this};
};

#endif  // HNS_BROWSER_UI_VIEWS_HNS_ACTIONS_HNS_ACTIONS_CONTAINER_H_
