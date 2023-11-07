/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/browser/ui/tabs/hns_vertical_tab_color_mixer.h"

#include "base/containers/fixed_flat_map.h"
#include "hns/browser/ui/color/hns_color_id.h"
#include "chrome/browser/ui/color/chrome_color_id.h"
#include "ui/color/color_mixer.h"
#include "ui/color/color_provider.h"
#include "ui/color/color_provider_key.h"
#include "ui/color/color_recipe.h"

namespace {

ChromeColorIds GetMappedChromeColorId(HnsColorIds hns_color_id) {
  static constexpr const auto kChromiumColorMap =
      base::MakeFixedFlatMap<HnsColorIds, ChromeColorIds>(
          // Note that we mapped inactive tab to active tab background and
          // vice versa. Vertical tabs are not in frame color, we should flip
          // them for our design goal.
          {
              {kColorHnsVerticalTabActiveBackground,
               kColorTabBackgroundInactiveFrameActive},
              {kColorHnsVerticalTabInactiveBackground,
               kColorTabBackgroundActiveFrameActive},
              {kColorHnsVerticalTabSeparator, kColorToolbarSeparator},
              {kColorHnsVerticalTabHeaderButtonColor,
               kColorToolbarButtonIcon},
              {kColorHnsVerticalTabNTBIconColor,
               kColorTabForegroundInactiveFrameActive},
              {kColorHnsVerticalTabNTBTextColor,
               kColorTabForegroundInactiveFrameActive},
              {kColorHnsVerticalTabNTBShortcutTextColor,
               kColorTabForegroundInactiveFrameActive},
          });
  return kChromiumColorMap.at(hns_color_id);
}

ui::ColorTransform GetCustomColorOrDefaultColor(
    const scoped_refptr<ui::ColorProviderKey::ThemeInitializerSupplier>&
        custom_theme,
    HnsColorIds color_id,
    SkColor color) {
  if (!custom_theme) {
    return {color};
  }

  auto chrome_color_id = GetMappedChromeColorId(color_id);
  if (custom_theme->GetColor(chrome_color_id, &color)) {
    return {color};
  }

  if (color_utils::HSL hsl; custom_theme->GetTint(chrome_color_id, &hsl)) {
    return {ui::HSLShift(color, hsl)};
  }

  return {chrome_color_id};
}

}  // namespace

namespace tabs {

void AddHnsVerticalTabLightThemeColorMixer(ui::ColorProvider* provider,
                                             const ui::ColorProviderKey& key) {
  ui::ColorMixer& mixer = provider->AddMixer();

  static constexpr const auto kDefaultColorMap =
      base::MakeFixedFlatMap<HnsColorIds, SkColor>({
          {kColorHnsVerticalTabActiveBackground, SK_ColorWHITE},
          {kColorHnsVerticalTabInactiveBackground,
           SkColorSetRGB(0xf3, 0xf3, 0xf3)},
          {kColorHnsVerticalTabSeparator,
           SkColorSetA(SK_ColorBLACK, 0.05 * 255)},
          {kColorHnsVerticalTabHeaderButtonColor,
           SkColorSetRGB(0x6B, 0x70, 0x84)},
          {kColorHnsVerticalTabNTBIconColor,
           SkColorSetARGB(0.6 * 255, 0x1D, 0x1F, 0x25)},
          {kColorHnsVerticalTabNTBTextColor, SkColorSetRGB(0x6B, 0x70, 0x84)},
          {kColorHnsVerticalTabNTBShortcutTextColor,
           SkColorSetRGB(0x85, 0x89, 0x89)},
      });
  for (const auto& [color_id, default_color] : kDefaultColorMap) {
    mixer[color_id] =
        GetCustomColorOrDefaultColor(key.custom_theme, color_id, default_color);
  }
}

void AddHnsVerticalTabDarkThemeColorMixer(ui::ColorProvider* provider,
                                            const ui::ColorProviderKey& key) {
  ui::ColorMixer& mixer = provider->AddMixer();

  static constexpr const auto kDefaultColorMap =
      base::MakeFixedFlatMap<HnsColorIds, SkColor>({
          {kColorHnsVerticalTabActiveBackground,
           SkColorSetA(SK_ColorBLACK, 0.3 * 255)},
          {kColorHnsVerticalTabInactiveBackground,
           SkColorSetRGB(0x30, 0x34, 0x43)},
          {kColorHnsVerticalTabSeparator,
           SkColorSetA(SK_ColorWHITE, 0.1 * 255)},
          {kColorHnsVerticalTabHeaderButtonColor, SK_ColorWHITE},
          {kColorHnsVerticalTabNTBIconColor,
           SkColorSetA(SK_ColorWHITE, 0.6 * 255)},
          {kColorHnsVerticalTabNTBTextColor, SkColorSetRGB(0x8C, 0x90, 0xA1)},
          {kColorHnsVerticalTabNTBShortcutTextColor,
           SkColorSetRGB(0x68, 0x6D, 0x7D)},
      });
  for (const auto& [color_id, default_color] : kDefaultColorMap) {
    auto color =
        GetCustomColorOrDefaultColor(key.custom_theme, color_id, default_color);
    if (color_id == kColorHnsVerticalTabActiveBackground ||
        color_id == kColorHnsVerticalTabInactiveBackground) {
      mixer[color_id] = ui::GetResultingPaintColor(
          /* foreground_transform= */ color,
          /* background_transform= */ kColorToolbar);
    } else {
      mixer[color_id] = color;
    }
  }
}

}  // namespace tabs
