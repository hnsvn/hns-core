/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/browser/ui/tabs/hns_tab_strip_model.h"

#include <algorithm>
#include <vector>

#include "hns/browser/ui/hns_browser_window.h"
#include "hns/components/constants/pref_names.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/ui/browser.h"
#include "chrome/browser/ui/browser_finder.h"
#include "chrome/browser/ui/browser_window.h"
#include "chrome/browser/ui/tabs/tab_strip_model_delegate.h"
#include "chrome/browser/ui/tabs/tab_strip_model_observer.h"
#include "components/prefs/pref_service.h"
#include "content/public/browser/web_contents.h"

HnsTabStripModel::HnsTabStripModel(
    TabStripModelDelegate* delegate,
    Profile* profile,
    TabGroupModelFactory* group_model_factory)
    : TabStripModel(delegate, profile, group_model_factory) {}
HnsTabStripModel::~HnsTabStripModel() = default;

void HnsTabStripModel::SelectRelativeTab(TabRelativeDirection direction,
                                           TabStripUserGestureDetails detail) {
  if (contents_data_.empty())
    return;

  bool is_mru_enabled = profile()->GetPrefs()->GetBoolean(kMRUCyclingEnabled);

  if (is_mru_enabled) {
    SelectMRUTab(direction, detail);
  } else {
    TabStripModel::SelectRelativeTab(direction, detail);
  }
}

void HnsTabStripModel::SelectMRUTab(TabRelativeDirection direction,
                                      TabStripUserGestureDetails detail) {
  if (mru_cycle_list_.empty()) {
    // Start cycling

    Browser* browser = chrome::FindBrowserWithWebContents(GetWebContentsAt(0));
    if (!browser)
      return;

    // Create a list of tab indexes sorted by time of last activation
    for (int i = 0; i < count(); ++i) {
      mru_cycle_list_.push_back(i);
    }

    std::sort(mru_cycle_list_.begin(), mru_cycle_list_.end(),
              [this](int a, int b) {
                return GetWebContentsAt(a)->GetLastActiveTime() >
                       GetWebContentsAt(b)->GetLastActiveTime();
              });

    // Tell the cycling controller that we start cycling to handle tabs keys
    static_cast<HnsBrowserWindow*>(browser->window())->StartTabCycling();
  }

  if (direction == TabRelativeDirection::kNext) {
    std::rotate(mru_cycle_list_.begin(), mru_cycle_list_.begin() + 1,
                mru_cycle_list_.end());
  } else {
    std::rotate(mru_cycle_list_.rbegin(), mru_cycle_list_.rbegin() + 1,
                mru_cycle_list_.rend());
  }

  ActivateTabAt(mru_cycle_list_[0], detail);
}

void HnsTabStripModel::StopMRUCycling() {
  mru_cycle_list_.clear();
}

std::vector<int> HnsTabStripModel::GetTabIndicesForCommandAt(int tab_index) {
  return TabStripModel::GetIndicesForCommand(tab_index);
}
