/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_UI_VIEWS_PROFILES_HNS_AVATAR_TOOLBAR_BUTTON_DELEGATE_H_
#define HNS_BROWSER_UI_VIEWS_PROFILES_HNS_AVATAR_TOOLBAR_BUTTON_DELEGATE_H_

#include "chrome/browser/ui/views/profiles/avatar_toolbar_button_delegate.h"

class HnsAvatarToolbarButtonDelegate : public AvatarToolbarButtonDelegate {
 public:
  using AvatarToolbarButtonDelegate::AvatarToolbarButtonDelegate;
  HnsAvatarToolbarButtonDelegate(AvatarToolbarButton* button,
                                   Browser* browser);
  HnsAvatarToolbarButtonDelegate(const HnsAvatarToolbarButtonDelegate&) =
      delete;
  HnsAvatarToolbarButtonDelegate& operator=(
      const HnsAvatarToolbarButtonDelegate&) = delete;
  ~HnsAvatarToolbarButtonDelegate() override = default;

  gfx::Image GetGaiaAccountImage() const;
};

#endif  // HNS_BROWSER_UI_VIEWS_PROFILES_HNS_AVATAR_TOOLBAR_BUTTON_DELEGATE_H_
