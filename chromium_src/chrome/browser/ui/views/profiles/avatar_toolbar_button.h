// Copyright (c) 2019 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef HNS_CHROMIUM_SRC_CHROME_BROWSER_UI_VIEWS_PROFILES_AVATAR_TOOLBAR_BUTTON_H_
#define HNS_CHROMIUM_SRC_CHROME_BROWSER_UI_VIEWS_PROFILES_AVATAR_TOOLBAR_BUTTON_H_

#define HNS_AVATAR_BUTTON_H friend class HnsAvatarToolbarButton;
#define GetAvatarIcon virtual GetAvatarIcon
#define GetAvatarTooltipText virtual GetAvatarTooltipText
#define AvatarToolbarButtonDelegate HnsAvatarToolbarButtonDelegate
#include "src/chrome/browser/ui/views/profiles/avatar_toolbar_button.h"  // IWYU pragma: export
#undef HNS_AVATAR_BUTTON_H
#undef GetAvatarIcon
#undef GetAvatarTooltipText
#undef AvatarToolbarButtonDelegate

#endif  // HNS_CHROMIUM_SRC_CHROME_BROWSER_UI_VIEWS_PROFILES_AVATAR_TOOLBAR_BUTTON_H_

