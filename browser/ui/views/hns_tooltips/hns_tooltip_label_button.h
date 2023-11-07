/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_UI_VIEWS_HNS_TOOLTIPS_HNS_TOOLTIP_LABEL_BUTTON_H_
#define HNS_BROWSER_UI_VIEWS_HNS_TOOLTIPS_HNS_TOOLTIP_LABEL_BUTTON_H_

#include <string>

#include "ui/views/controls/button/label_button.h"

namespace hns_tooltips {

class HnsTooltipLabelButton : public views::LabelButton {
 public:
  // Creates a HnsTooltipLabelButton with pressed events sent to |callback|
  // and label |text|. |button_context| is a value from
  // views::style::TextContext and determines the appearance of |text|.
  explicit HnsTooltipLabelButton(
      PressedCallback callback = PressedCallback(),
      const std::u16string& text = std::u16string(),
      int button_context = views::style::CONTEXT_BUTTON);
  ~HnsTooltipLabelButton() override;

  HnsTooltipLabelButton(const HnsTooltipLabelButton&) = delete;
  HnsTooltipLabelButton& operator=(const HnsTooltipLabelButton&) = delete;

  // views::LabelButton:
  ui::Cursor GetCursor(const ui::MouseEvent& event) override;
};

}  // namespace hns_tooltips

#endif  // HNS_BROWSER_UI_VIEWS_HNS_TOOLTIPS_HNS_TOOLTIP_LABEL_BUTTON_H_
