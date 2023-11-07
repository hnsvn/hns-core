/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_UI_VIEWS_BUBBLE_HNS_WEBUI_BUBBLE_MANAGER_H_
#define HNS_BROWSER_UI_VIEWS_BUBBLE_HNS_WEBUI_BUBBLE_MANAGER_H_

#include "chrome/browser/ui/views/bubble/webui_bubble_manager.h"

// A subclass of WebUIBubbleManagerT that allows customization of the bubble
// border radius and other aspects of the rendered bubble view. Use exactly like
// WebUIBubbleManagerT, or subclass if a different customization behavior is
// required.
template <typename T>
class HnsWebUIBubbleManager : public WebUIBubbleManagerT<T> {
 public:
  using WebUIBubbleManagerT<T>::WebUIBubbleManagerT;

  ~HnsWebUIBubbleManager() override = default;

 protected:
  // Allows customization of the rendered bubble dialog view.
  virtual void CustomizeBubbleDialogView(WebUIBubbleDialogView& bubble_view) {
    bubble_view.SetPaintClientToLayer(true);
    bubble_view.set_use_round_corners(true);
    bubble_view.set_corner_radius(16);
  }

 private:
  void HnsCustomizeBubbleDialogView(
      WebUIBubbleDialogView& bubble_view) override {
    CustomizeBubbleDialogView(bubble_view);
  }
};

#endif  // HNS_BROWSER_UI_VIEWS_BUBBLE_HNS_WEBUI_BUBBLE_MANAGER_H_
