/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_CHROMIUM_SRC_THIRD_PARTY_BLINK_RENDERER_MODULES_KEYBOARD_NAVIGATOR_KEYBOARD_H_
#define HNS_CHROMIUM_SRC_THIRD_PARTY_BLINK_RENDERER_MODULES_KEYBOARD_NAVIGATOR_KEYBOARD_H_

#define keyboard                     \
  keyboard_ChromiumImpl(Navigator&); \
  static Keyboard* keyboard

#include "src/third_party/blink/renderer/modules/keyboard/navigator_keyboard.h"  // IWYU pragma: export
#undef keyboard

#endif  // HNS_CHROMIUM_SRC_THIRD_PARTY_BLINK_RENDERER_MODULES_KEYBOARD_NAVIGATOR_KEYBOARD_H_
