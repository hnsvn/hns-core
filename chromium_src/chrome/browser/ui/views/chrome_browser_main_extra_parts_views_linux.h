/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_CHROMIUM_SRC_CHROME_BROWSER_UI_VIEWS_CHROME_BROWSER_MAIN_EXTRA_PARTS_VIEWS_LINUX_H_
#define HNS_CHROMIUM_SRC_CHROME_BROWSER_UI_VIEWS_CHROME_BROWSER_MAIN_EXTRA_PARTS_VIEWS_LINUX_H_

#if defined(USE_DBUS)
namespace ui {
class HnsDarkModeManagerLinux;
}  // namespace ui

// Remove this overriding when linux also supports system default theme option.
#define DarkModeManagerLinux HnsDarkModeManagerLinux
#endif

#include "src/chrome/browser/ui/views/chrome_browser_main_extra_parts_views_linux.h"  // IWYU pragma: export

#if defined(USE_DBUS)
#undef DarkModeManagerLinux
#endif

#endif  // HNS_CHROMIUM_SRC_CHROME_BROWSER_UI_VIEWS_CHROME_BROWSER_MAIN_EXTRA_PARTS_VIEWS_LINUX_H_
