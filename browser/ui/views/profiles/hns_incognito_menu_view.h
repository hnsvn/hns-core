/* Copyright 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_UI_VIEWS_PROFILES_HNS_INCOGNITO_MENU_VIEW_H_
#define HNS_BROWSER_UI_VIEWS_PROFILES_HNS_INCOGNITO_MENU_VIEW_H_

#include "chrome/browser/ui/views/profiles/incognito_menu_view.h"

class HnsIncognitoMenuView : public IncognitoMenuView {
 public:
  using IncognitoMenuView::IncognitoMenuView;

  HnsIncognitoMenuView(const HnsIncognitoMenuView&) = delete;
  HnsIncognitoMenuView& operator=(const HnsIncognitoMenuView&) = delete;
  ~HnsIncognitoMenuView() override = default;

  // ProfileMenuViewBase:
  void BuildMenu() override;
  void AddedToWidget() override;

 private:
  friend class IncognitoMenuView;

  // views::BubbleDialogDelegateView:
  std::u16string GetAccessibleWindowTitle() const override;

  // Button actions.
  void OnExitButtonClicked() override;

  void AddTorButton();
  void OnTorProfileButtonClicked();
};

#endif  // HNS_BROWSER_UI_VIEWS_PROFILES_HNS_INCOGNITO_MENU_VIEW_H_
