/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_CHROMIUM_SRC_CHROME_BROWSER_UI_TABS_TAB_STRIP_MODEL_H_
#define HNS_CHROMIUM_SRC_CHROME_BROWSER_UI_TABS_TAB_STRIP_MODEL_H_

#define SelectRelativeTab virtual SelectRelativeTab
#define TAB_STRIP_MODEL_H_ friend class HnsTabStripModel;
#define IsReadLaterSupportedForAny virtual IsReadLaterSupportedForAny

#include "src/chrome/browser/ui/tabs/tab_strip_model.h"  // IWYU pragma: export
#undef IsReadLaterSupportedForAny
#undef SelectRelativeTab
#undef TAB_STRIP_MODEL_H_

#endif  // HNS_CHROMIUM_SRC_CHROME_BROWSER_UI_TABS_TAB_STRIP_MODEL_H_
