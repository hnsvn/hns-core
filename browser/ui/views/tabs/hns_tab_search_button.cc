/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * you can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/browser/ui/views/tabs/hns_tab_search_button.h"

#include <algorithm>
#include <memory>

#include "hns/browser/ui/views/hns_tab_search_bubble_host.h"
#include "hns/browser/ui/views/tabs/hns_new_tab_button.h"
#include "chrome/browser/ui/views/chrome_layout_provider.h"
#include "chrome/browser/ui/views/tabs/new_tab_button.h"
#include "chrome/browser/ui/views/tabs/tab_strip_controller.h"
#include "ui/base/metadata/metadata_impl_macros.h"
#include "ui/gfx/geometry/point_f.h"
#include "ui/gfx/geometry/rect.h"
#include "ui/gfx/geometry/size.h"
#include "ui/gfx/geometry/skia_conversions.h"
#include "ui/views/layout/layout_provider.h"

HnsTabSearchButton::HnsTabSearchButton(TabStrip* tab_strip)
    : TabSearchButton(tab_strip) {
  tab_search_bubble_host_ = std::make_unique<HnsTabSearchBubbleHost>(
      this, tab_strip->controller()->GetProfile());
}

HnsTabSearchButton::~HnsTabSearchButton() = default;

gfx::Size HnsTabSearchButton::CalculatePreferredSize() const {
  return HnsNewTabButton::kButtonSize;
}

void HnsTabSearchButton::SetBubbleArrow(views::BubbleBorder::Arrow arrow) {
  static_cast<HnsTabSearchBubbleHost*>(tab_search_bubble_host_.get())
      ->SetBubbleArrow(arrow);
}

int HnsTabSearchButton::GetCornerRadius() const {
  return ChromeLayoutProvider::Get()->GetCornerRadiusMetric(
      views::Emphasis::kMaximum, GetContentsBounds().size());
}

BEGIN_METADATA(HnsTabSearchButton, TabSearchButton)
END_METADATA
