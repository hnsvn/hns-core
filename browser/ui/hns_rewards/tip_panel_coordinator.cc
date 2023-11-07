/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/browser/ui/hns_rewards/tip_panel_coordinator.h"

#include <string>
#include <utility>

#include "hns/browser/ui/hns_rewards/rewards_panel_coordinator.h"
#include "hns/components/hns_rewards/browser/rewards_service.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/ui/browser_window.h"

namespace hns_rewards {

namespace {

void OpenRewardsPanel(Browser* browser, bool inline_tip) {
  if (auto* coordinator = RewardsPanelCoordinator::FromBrowser(browser)) {
    if (inline_tip) {
      coordinator->ShowInlineTipView();
    } else {
      coordinator->OpenRewardsPanel();
    }
  }
}

}  // namespace

TipPanelCoordinator::TipPanelCoordinator(Browser* browser,
                                         RewardsService* rewards_service)
    : BrowserUserData<TipPanelCoordinator>(*browser),
      rewards_service_(rewards_service) {
  DCHECK(rewards_service_);
}

TipPanelCoordinator::~TipPanelCoordinator() = default;

void TipPanelCoordinator::ShowPanelForPublisher(
    const std::string& publisher_id) {
  rewards_service_->GetUserType(
      base::BindOnce(&TipPanelCoordinator::GetUserTypeCallback,
                     weak_factory_.GetWeakPtr(), publisher_id, false));
}

void TipPanelCoordinator::ShowPanelForInlineTip(
    const std::string& publisher_id) {
  rewards_service_->GetUserType(
      base::BindOnce(&TipPanelCoordinator::GetUserTypeCallback,
                     weak_factory_.GetWeakPtr(), publisher_id, true));
}

void TipPanelCoordinator::AddObserver(Observer* observer) {
  observers_.AddObserver(observer);
}

void TipPanelCoordinator::RemoveObserver(Observer* observer) {
  observers_.RemoveObserver(observer);
}

void TipPanelCoordinator::GetUserTypeCallback(const std::string& publisher_id,
                                              bool inline_tip,
                                              mojom::UserType user_type) {
  // If the user is not "connected" (i.e. if they have not linked an external
  // wallet and they are not a "legacy" anonymous user), then open the Rewards
  // panel instead.
  if (user_type == mojom::UserType::kUnconnected) {
    OpenRewardsPanel(&GetBrowser(), inline_tip);
    return;
  }

  rewards_service_->IsPublisherRegistered(
      publisher_id,
      base::BindOnce(&TipPanelCoordinator::IsPublisherRegisteredCallback,
                     weak_factory_.GetWeakPtr(), publisher_id, inline_tip));
}

void TipPanelCoordinator::IsPublisherRegisteredCallback(
    const std::string& publisher_id,
    bool inline_tip,
    bool is_publisher_registered) {
  // If the creator is not "registered" (and therefore has no banner information
  // to display), then open the Rewards panel instead.
  if (!is_publisher_registered) {
    OpenRewardsPanel(&GetBrowser(), inline_tip);
    return;
  }

  OpenPanel(publisher_id);
}

void TipPanelCoordinator::OpenPanel(const std::string& publisher_id) {
  if (GetBrowser().window()->IsMinimized()) {
    GetBrowser().window()->Restore();
  }

  publisher_id_ = publisher_id;

  for (auto& observer : observers_) {
    observer.OnTipPanelRequested(publisher_id);
  }
}

BROWSER_USER_DATA_KEY_IMPL(TipPanelCoordinator);

}  // namespace hns_rewards
