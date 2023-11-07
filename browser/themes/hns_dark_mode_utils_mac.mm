/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/browser/themes/hns_dark_mode_utils.h"

#import <Cocoa/Cocoa.h>

#include "hns/browser/themes/hns_dark_mode_utils_internal.h"
#include "ui/native_theme/native_theme.h"

namespace dark_mode {

void SetSystemDarkMode(HnsDarkModeType type) {
  if (type == HnsDarkModeType::HNS_DARK_MODE_TYPE_DEFAULT) {
    DCHECK(
        ui::NativeTheme::GetInstanceForNativeUi()->SystemDarkModeSupported());
    [NSApp setAppearance:nil];
    return;
  }

  if (@available(macOS 10.14, *)) {
    NSAppearanceName new_appearance_name =
        type == HnsDarkModeType::HNS_DARK_MODE_TYPE_DARK ?
            NSAppearanceNameDarkAqua : NSAppearanceNameAqua;
    [NSApp setAppearance:[NSAppearance appearanceNamed:new_appearance_name]];
  } else {
    internal::SetSystemDarkModeForNonDefaultMode(
        type == HnsDarkModeType::HNS_DARK_MODE_TYPE_DARK);
  }
}

}  // namespace dark_mode
