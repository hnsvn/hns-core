/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_UI_TABS_HNS_TAB_STRIP_MODEL_H_
#define HNS_BROWSER_UI_TABS_HNS_TAB_STRIP_MODEL_H_

#include <vector>

#include "chrome/browser/ui/tabs/tab_strip_model.h"

class HnsTabStripModel : public TabStripModel {
 public:
  explicit HnsTabStripModel(TabStripModelDelegate* delegate,
                              Profile* profile,
                              TabGroupModelFactory* group_model_factory);

  ~HnsTabStripModel() override;

  HnsTabStripModel(const HnsTabStripModel&) = delete;
  HnsTabStripModel operator=(const HnsTabStripModel&) = delete;

  void SelectRelativeTab(TabRelativeDirection direction,
                         TabStripUserGestureDetails detail) override;

  // Set the next tab when doing a MRU cycling with Ctrl-tab
  void SelectMRUTab(
      TabRelativeDirection direction,
      TabStripUserGestureDetails detail = TabStripUserGestureDetails(
          TabStripUserGestureDetails::GestureType::kOther));

  // Stop MRU cycling, called when releasing the Ctrl key
  void StopMRUCycling();

  // Exposes a |TabStripModel| api to |HnsTabMenuModel|.
  std::vector<int> GetTabIndicesForCommandAt(int tab_index);

 private:
  // List of tab indexes sorted by most recently used
  std::vector<int> mru_cycle_list_;
};

#endif  // HNS_BROWSER_UI_TABS_HNS_TAB_STRIP_MODEL_H_
