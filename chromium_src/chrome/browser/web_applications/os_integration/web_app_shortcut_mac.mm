/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

namespace base {
class FilePath;
}  // namespace base

namespace {
base::FilePath GetLocalizableHnsAppShortcutsSubdirName();
}

#include "src/chrome/browser/web_applications/os_integration/web_app_shortcut_mac.mm"

namespace {
base::FilePath GetLocalizableHnsAppShortcutsSubdirName() {
  // clang-format off
  static const char kHnsBrowserDevelopmentAppDirName[] =
      "Hns Browser Development Apps.localized";
  static const char kHnsBrowserAppDirName[] =
      "Hns Browser Apps.localized";
  static const char kHnsBrowserBetaAppDirName[] =
      "Hns Browser Beta Apps.localized";
  static const char kHnsBrowserDevAppDirName[] =
      "Hns Browser Dev Apps.localized";
  static const char kHnsBrowserNightlyAppDirName[] =
      "Hns Browser Nightly Apps.localized";
  // clang-format on

  switch (chrome::GetChannel()) {
    case version_info::Channel::STABLE:
      return base::FilePath(kHnsBrowserAppDirName);
    case version_info::Channel::BETA:
      return base::FilePath(kHnsBrowserBetaAppDirName);
    case version_info::Channel::DEV:
      return base::FilePath(kHnsBrowserDevAppDirName);
    case version_info::Channel::CANARY:
      return base::FilePath(kHnsBrowserNightlyAppDirName);
    case version_info::Channel::UNKNOWN:
      return base::FilePath(kHnsBrowserDevelopmentAppDirName);
    default:
      NOTREACHED();
      return base::FilePath(kHnsBrowserDevelopmentAppDirName);
  }
}
}  // namespace
