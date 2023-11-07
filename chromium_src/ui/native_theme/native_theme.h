// Copyright (c) 2019 The Hns Authors
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef HNS_CHROMIUM_SRC_UI_NATIVE_THEME_NATIVE_THEME_H_
#define HNS_CHROMIUM_SRC_UI_NATIVE_THEME_NATIVE_THEME_H_

#define GetSystemButtonPressedColor                                   \
  GetSystemButtonPressedColor_ChromiumImpl(SkColor base_color) const; \
  friend void SetUseDarkColors(bool dark_mode);                       \
  friend void ReCalcAndSetPreferredColorScheme();                     \
  static NativeTheme* HnsGetInstanceForNativeUi();                  \
  virtual SkColor GetSystemButtonPressedColor

#include "src/ui/native_theme/native_theme.h"  // IWYU pragma: export
#undef GetSystemButtonPressedColor

#endif  // HNS_CHROMIUM_SRC_UI_NATIVE_THEME_NATIVE_THEME_H_
