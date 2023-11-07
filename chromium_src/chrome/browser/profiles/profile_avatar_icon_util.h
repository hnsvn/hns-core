// Copyright 2019 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef HNS_CHROMIUM_SRC_CHROME_BROWSER_PROFILES_PROFILE_AVATAR_ICON_UTIL_H_
#define HNS_CHROMIUM_SRC_CHROME_BROWSER_PROFILES_PROFILE_AVATAR_ICON_UTIL_H_

#include "src/chrome/browser/profiles/profile_avatar_icon_util.h"  // IWYU pragma: export

namespace profiles {
  // Access original value for tests
  extern const size_t kHnsDefaultAvatarIconsCount;
  // Provide direct access to custom implementation
  base::Value::Dict GetDefaultProfileAvatarIconAndLabel_Hns(
      SkColor fill_color,
      SkColor stroke_color,
      bool selected);
}  // namespace profiles

#endif  // HNS_CHROMIUM_SRC_CHROME_BROWSER_PROFILES_PROFILE_AVATAR_ICON_UTIL_H_
