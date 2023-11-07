/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_UI_COLOR_HNS_COLOR_MIXER_H_
#define HNS_BROWSER_UI_COLOR_HNS_COLOR_MIXER_H_

#include "third_party/skia/include/core/SkColor.h"
#include "ui/color/color_provider_key.h"

namespace ui {
class ColorProvider;
}  // namespace ui

// Exposed for testing.
SkColor GetLocationBarBackground(bool dark, bool priv, bool hover);
SkColor GetOmniboxResultBackground(int id, bool dark, bool priv);

// Handling normal profile's dark or light theme.
void AddHnsThemeColorMixer(ui::ColorProvider* provider,
                             const ui::ColorProviderKey& key);
void AddBravifiedChromeThemeColorMixer(ui::ColorProvider* provider,
                                       const ui::ColorProviderKey& key);
void AddPrivateThemeColorMixer(ui::ColorProvider* provider,
                               const ui::ColorProviderKey& key);
void AddTorThemeColorMixer(ui::ColorProvider* provider,
                           const ui::ColorProviderKey& key);
void AddHnsOmniboxLightThemeColorMixer(ui::ColorProvider* provider,
                                         const ui::ColorProviderKey& key);
void AddHnsOmniboxDarkThemeColorMixer(ui::ColorProvider* provider,
                                        const ui::ColorProviderKey& key);
void AddHnsOmniboxPrivateThemeColorMixer(ui::ColorProvider* provider,
                                           const ui::ColorProviderKey& key);

#endif  // HNS_BROWSER_UI_COLOR_HNS_COLOR_MIXER_H_
