/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/browser/ui/views/hns_tooltips/hns_tooltip_label_button.h"

#include "ui/base/cursor/cursor.h"

namespace hns_tooltips {

HnsTooltipLabelButton::HnsTooltipLabelButton(PressedCallback callback,
                                                 const std::u16string& text,
                                                 int button_context)
    : LabelButton(callback, text, button_context) {}

HnsTooltipLabelButton::~HnsTooltipLabelButton() = default;

ui::Cursor HnsTooltipLabelButton::GetCursor(const ui::MouseEvent& event) {
  if (!GetEnabled())
    return ui::Cursor();
  return ui::mojom::CursorType::kHand;
}

}  // namespace hns_tooltips
