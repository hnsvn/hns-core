/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_UI_VIEWS_PROFILES_HNS_AVATAR_TOOLBAR_BUTTON_H_
#define HNS_BROWSER_UI_VIEWS_PROFILES_HNS_AVATAR_TOOLBAR_BUTTON_H_

#include "chrome/browser/ui/views/profiles/avatar_toolbar_button.h"

class BrowserView;

class HnsAvatarToolbarButton : public AvatarToolbarButton {
 public:
  explicit HnsAvatarToolbarButton(BrowserView* browser_view);
  HnsAvatarToolbarButton(const HnsAvatarToolbarButton&) = delete;
  HnsAvatarToolbarButton& operator=(const HnsAvatarToolbarButton&) = delete;
  ~HnsAvatarToolbarButton() override;

  AvatarToolbarButton::State GetAvatarButtonState() const;

  // ToolbarButton:
  void SetHighlight(const std::u16string& highlight_text,
                    absl::optional<SkColor> highlight_color) override;
  void UpdateColorsAndInsets() override;

 private:
  // AvatarToolbarButton:
  ui::ImageModel GetAvatarIcon(
      ButtonState state,
      const gfx::Image& profile_identity_image) const override;
  std::u16string GetAvatarTooltipText() const override;
  int GetWindowCount() const;

  base::WeakPtrFactory<HnsAvatarToolbarButton> weak_ptr_factory_{this};
};

#endif  // HNS_BROWSER_UI_VIEWS_PROFILES_HNS_AVATAR_TOOLBAR_BUTTON_H_
