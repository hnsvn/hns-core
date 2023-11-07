/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_CHROMIUM_SRC_CHROME_APP_ANDROID_CHROME_MAIN_DELEGATE_ANDROID_H_
#define HNS_CHROMIUM_SRC_CHROME_APP_ANDROID_CHROME_MAIN_DELEGATE_ANDROID_H_

#include "hns/app/hns_main_delegate.h"

#define ChromeMainDelegate HnsMainDelegate
#include "src/chrome/app/android/chrome_main_delegate_android.h"  // IWYU pragma: export
#undef ChromeMainDelegate

#endif  // HNS_CHROMIUM_SRC_CHROME_APP_ANDROID_CHROME_MAIN_DELEGATE_ANDROID_H_
