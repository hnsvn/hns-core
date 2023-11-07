/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/browser/ui/color/color_palette.h"
#include "hns/browser/ui/views/tabs/hns_tab_strip.h"
#include "testing/gtest/include/gtest/gtest.h"
#include "ui/gfx/color_utils.h"

TEST(ColorPaletteTest, LightThemeMinimumContrast) {
  // Re-visit kHnsMinimumContrastRatioForOutlines when contrast ratio between
  // kLightToolbar and kLightFrame has lowered.
  EXPECT_GT(color_utils::GetContrastRatio(kLightToolbar, kLightFrame),
            HnsTabStrip::kHnsMinimumContrastRatioForOutlines);
}
