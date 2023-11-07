/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_CHROMIUM_SRC_CHROME_COMMON_CHROME_ICON_RESOURCES_WIN_H_
#define HNS_CHROMIUM_SRC_CHROME_COMMON_CHROME_ICON_RESOURCES_WIN_H_

namespace icon_resources {

// This file contains the indices of icon resources in hns_exe.rc.

enum {
  // The main application icon is always index 0.
  kApplicationIndex = 0,

#if defined(OFFICIAL_BUILD)
  // Legacy indices that are no longer used.
  kApplication2Index = 1,
  kApplication3Index = 2,
  kApplication4Index = 3,

  // The Hns Canary application icon.
  kSxSApplicationIndex = 4,

  // The Hns App Launcher icon.
  kAppLauncherIndex = 5,

  // The Hns App Launcher Canary icon.
  kSxSAppLauncherIndex = 6,

  // The Hns incognito icon.
  kIncognitoIndex = 7,

  // The Hns Dev application icon.
  kDevApplicationIndex = 8,

  // The Hns Beta application icon.
  kBetaApplicationIndex = 9,

  // The various file type icon(ex, pdf).
  kFileTypeIndex = 10,
  kBetaFileTypeIndex = 11,
  kDevFileTypeIndex = 12,
  kSxSFileTypeIndex = 13,
#else
  // The Hns Developer App Launcher icon.
  kAppLauncherIndex = 1,

  // The Hns Developer incognito icon.
  kIncognitoIndex = 2,
#endif
};

}  // namespace icon_resources

#endif  // HNS_CHROMIUM_SRC_CHROME_COMMON_CHROME_ICON_RESOURCES_WIN_H_
