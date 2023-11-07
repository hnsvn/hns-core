/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/browser/ui/hns_tooltips/hns_tooltip_attributes.h"

namespace hns_tooltips {

HnsTooltipAttributes::HnsTooltipAttributes(
    const std::u16string& title,
    const std::u16string& body,
    const std::u16string& ok_button_text,
    const std::u16string& cancel_button_text)
    : title_(title),
      body_(body),
      ok_button_text_(ok_button_text),
      cancel_button_text_(cancel_button_text) {
  if (!cancel_button_text.empty()) {
    cancel_button_enabled_ = true;
  }
}

HnsTooltipAttributes::~HnsTooltipAttributes() = default;

HnsTooltipAttributes::HnsTooltipAttributes(
    const HnsTooltipAttributes& other) = default;

HnsTooltipAttributes& HnsTooltipAttributes::operator=(
    const HnsTooltipAttributes& other) = default;

}  // namespace hns_tooltips
