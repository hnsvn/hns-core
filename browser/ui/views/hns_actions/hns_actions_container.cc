/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/browser/ui/views/hns_actions/hns_actions_container.h"

#include <memory>
#include <utility>

#include "base/feature_list.h"
#include "hns/browser/hns_rewards/rewards_util.h"
#include "hns/browser/ui/views/hns_actions/hns_rewards_action_view.h"
#include "hns/browser/ui/views/hns_actions/hns_shields_action_view.h"
#include "hns/browser/ui/views/rounded_separator.h"
#include "hns/components/hns_rewards/common/pref_names.h"
#include "hns/components/constants/pref_names.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/ui/browser.h"
#include "chrome/browser/ui/layout_constants.h"
#include "components/prefs/pref_service.h"
#include "ui/views/layout/box_layout.h"
#include "ui/views/view.h"

namespace {

constexpr gfx::Size kToolbarActionSize(34, 24);

}  // namespace

HnsActionsContainer::HnsActionsContainer(Browser* browser, Profile* profile)
    : browser_(browser) {}

HnsActionsContainer::~HnsActionsContainer() = default;

void HnsActionsContainer::Init() {
  // automatic layout
  auto vertical_container_layout = std::make_unique<views::BoxLayout>(
      views::BoxLayout::Orientation::kHorizontal);
  vertical_container_layout->set_main_axis_alignment(
      views::BoxLayout::MainAxisAlignment::kCenter);
  vertical_container_layout->set_cross_axis_alignment(
      views::BoxLayout::CrossAxisAlignment::kCenter);
  SetLayoutManager(std::move(vertical_container_layout));

  // children
  RoundedSeparator* hns_button_separator_ = new RoundedSeparator();
  // TODO(petemill): theme color
  hns_button_separator_->SetColor(SkColorSetRGB(0xb2, 0xb5, 0xb7));
  constexpr int kSeparatorMargin = 3;
  constexpr int kSeparatorWidth = 1;
  hns_button_separator_->SetPreferredSize(
      gfx::Size(kSeparatorWidth + kSeparatorMargin * 2,
                GetLayoutConstant(LOCATION_BAR_ICON_SIZE)));
  // separator left & right margin
  hns_button_separator_->SetBorder(views::CreateEmptyBorder(
      gfx::Insets::TLBR(0, kSeparatorMargin, 0, kSeparatorMargin)));
  // Just in case the extensions load before this function does (not likely!)
  // make sure separator is at index 0
  AddChildViewAt(hns_button_separator_, 0);
  AddActionViewForShields();
  AddActionViewForRewards();

  // React to Hns Rewards preferences changes.
  show_hns_rewards_button_.Init(
      hns_rewards::prefs::kShowLocationBarButton,
      browser_->profile()->GetPrefs(),
      base::BindRepeating(
          &HnsActionsContainer::OnHnsRewardsPreferencesChanged,
          base::Unretained(this)));
}

bool HnsActionsContainer::ShouldShowHnsRewardsAction() const {
  if (!hns_rewards::IsSupportedForProfile(browser_->profile())) {
    return false;
  }
  const PrefService* prefs = browser_->profile()->GetPrefs();
  return prefs->GetBoolean(hns_rewards::prefs::kShowLocationBarButton);
}

void HnsActionsContainer::AddActionViewForShields() {
  shields_action_btn_ =
      AddChildViewAt(std::make_unique<HnsShieldsActionView>(
                         *browser_->profile(), *browser_->tab_strip_model()),
                     1);
  shields_action_btn_->SetPreferredSize(kToolbarActionSize);
  shields_action_btn_->Init();
}

void HnsActionsContainer::AddActionViewForRewards() {
  auto button = std::make_unique<HnsRewardsActionView>(browser_);
  rewards_action_btn_ = AddChildViewAt(std::move(button), 2);
  rewards_action_btn_->SetPreferredSize(kToolbarActionSize);
  rewards_action_btn_->SetVisible(ShouldShowHnsRewardsAction());
  rewards_action_btn_->Update();
}

void HnsActionsContainer::Update() {
  if (shields_action_btn_) {
    shields_action_btn_->Update();
  }

  if (rewards_action_btn_) {
    rewards_action_btn_->Update();
  }

  UpdateVisibility();
  Layout();
}

void HnsActionsContainer::UpdateVisibility() {
  bool can_show = false;

  if (shields_action_btn_) {
    can_show = shields_action_btn_->GetVisible();
  }

  if (rewards_action_btn_) {
    can_show = can_show || rewards_action_btn_->GetVisible();
  }

  // If no buttons are visible, then we want to hide this view so that the
  // separator is not displayed.
  SetVisible(!should_hide_ && can_show);
}

void HnsActionsContainer::SetShouldHide(bool should_hide) {
  should_hide_ = should_hide;
  Update();
}

void HnsActionsContainer::ChildPreferredSizeChanged(views::View* child) {
  PreferredSizeChanged();
}

// Hns Rewards preferences change observers callback
void HnsActionsContainer::OnHnsRewardsPreferencesChanged() {
  if (rewards_action_btn_) {
    rewards_action_btn_->SetVisible(ShouldShowHnsRewardsAction());
  }
}
