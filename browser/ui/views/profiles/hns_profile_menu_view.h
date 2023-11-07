/* Copyright 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_UI_VIEWS_PROFILES_HNS_PROFILE_MENU_VIEW_H_
#define HNS_BROWSER_UI_VIEWS_PROFILES_HNS_PROFILE_MENU_VIEW_H_

#include "chrome/browser/ui/views/profiles/profile_menu_view.h"

class HnsProfileMenuView : public ProfileMenuView {
 public:
  HnsProfileMenuView(const HnsProfileMenuView&) = delete;
  HnsProfileMenuView& operator=(const HnsProfileMenuView&) = delete;
  ~HnsProfileMenuView() override = default;

 private:
  friend class ProfileMenuView;
  using ProfileMenuView::ProfileMenuView;

  // Helper methods for building the menu.
  void BuildIdentity() override;
  void BuildAutofillButtons() override;
  void BuildSyncInfo() override;
  void BuildFeatureButtons() override;
  gfx::ImageSkia GetSyncIcon() const override;
};

#endif  // HNS_BROWSER_UI_VIEWS_PROFILES_HNS_PROFILE_MENU_VIEW_H_
