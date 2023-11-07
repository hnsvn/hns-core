/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_CHROMIUM_SRC_CHROME_BROWSER_UI_BROWSER_TAB_STRIP_MODEL_DELEGATE_H_
#define HNS_CHROMIUM_SRC_CHROME_BROWSER_UI_BROWSER_TAB_STRIP_MODEL_DELEGATE_H_

#define CloseFrame                         \
  CloseFrame_Unused();                     \
  friend class HnsTabStripModelDelegate; \
  void CloseFrame

#include "src/chrome/browser/ui/browser_tab_strip_model_delegate.h"  // IWYU pragma: export

#undef CloseFrame

#endif  // HNS_CHROMIUM_SRC_CHROME_BROWSER_UI_BROWSER_TAB_STRIP_MODEL_DELEGATE_H_
