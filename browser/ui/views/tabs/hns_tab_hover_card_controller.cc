// Copyright (c) 2022 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#include "hns/browser/ui/views/tabs/hns_tab_hover_card_controller.h"

#include <memory>

#include "base/functional/bind.h"
#include "hns/browser/ui/tabs/hns_tab_prefs.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/ui/browser.h"
#include "chrome/browser/ui/views/tabs/tab.h"
#include "chrome/browser/ui/views/tabs/tab_hover_card_bubble_view.h"
#include "chrome/browser/ui/views/tabs/tab_hover_card_controller.h"
#include "chrome/browser/ui/views/tabs/tab_hover_card_thumbnail_observer.h"
#include "ui/views/bubble/bubble_border.h"

HnsTabHoverCardController::~HnsTabHoverCardController() = default;

void HnsTabHoverCardController::SetIsVerticalTabs(bool is_vertical_tabs) {
  if (std::exchange(is_vertical_tabs_, is_vertical_tabs) == is_vertical_tabs) {
    return;
  }

  UpdateHoverCardArrow();
}

void HnsTabHoverCardController::UpdateHoverCardArrow() {
  if (hover_card_) {
    hover_card_->SetArrow(is_vertical_tabs_ ? views::BubbleBorder::LEFT_TOP
                                            : views::BubbleBorder::TOP_CENTER);
  }
}

void HnsTabHoverCardController::CreateHoverCard(Tab* tab) {
  TabHoverCardController::CreateHoverCard(tab);

  if (!thumbnail_observer_ &&
      hns_tabs::AreCardPreviewsEnabled(
          tab->controller()->GetBrowser()->profile()->GetPrefs())) {
    thumbnail_observer_ = std::make_unique<TabHoverCardThumbnailObserver>();
    thumbnail_subscription_ = thumbnail_observer_->AddCallback(
        base::BindRepeating(&TabHoverCardController::OnPreviewImageAvailable,
                            weak_ptr_factory_.GetWeakPtr()));
  }

  UpdateHoverCardArrow();
}
