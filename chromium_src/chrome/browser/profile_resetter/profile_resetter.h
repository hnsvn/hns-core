/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_CHROMIUM_SRC_CHROME_BROWSER_PROFILE_RESETTER_PROFILE_RESETTER_H_
#define HNS_CHROMIUM_SRC_CHROME_BROWSER_PROFILE_RESETTER_PROFILE_RESETTER_H_

#define ResetDefaultSearchEngine     \
  UnUsed() {}                        \
  friend class HnsProfileResetter; \
  virtual void ResetDefaultSearchEngine

#include "src/chrome/browser/profile_resetter/profile_resetter.h"  // IWYU pragma: export

#undef ResetDefaultSearchEngine

#endif  // HNS_CHROMIUM_SRC_CHROME_BROWSER_PROFILE_RESETTER_PROFILE_RESETTER_H_
