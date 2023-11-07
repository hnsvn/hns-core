/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_UI_COLOR_PLAYLIST_PLAYLIST_COLOR_MIXER_H_
#define HNS_BROWSER_UI_COLOR_PLAYLIST_PLAYLIST_COLOR_MIXER_H_

#include "hns/browser/ui/color/leo/colors.h"
#include "ui/color/color_provider_key.h"

namespace ui {
class ColorProvider;
}  // namespace ui

namespace playlist {

void AddThemeColorMixer(ui::ColorProvider* provider,
                        leo::Theme theme,
                        const ui::ColorProviderKey& key);

}  // namespace playlist

#endif  // HNS_BROWSER_UI_COLOR_PLAYLIST_PLAYLIST_COLOR_MIXER_H_
