/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_UI_TABS_HNS_VERTICAL_TAB_COLOR_MIXER_H_
#define HNS_BROWSER_UI_TABS_HNS_VERTICAL_TAB_COLOR_MIXER_H_

#include "ui/color/color_provider_key.h"

namespace tabs {

void AddHnsVerticalTabLightThemeColorMixer(ui::ColorProvider* provider,
                                             const ui::ColorProviderKey& key);
void AddHnsVerticalTabDarkThemeColorMixer(ui::ColorProvider* provider,
                                            const ui::ColorProviderKey& key);

}  // namespace tabs

#endif  // HNS_BROWSER_UI_TABS_HNS_VERTICAL_TAB_COLOR_MIXER_H_
