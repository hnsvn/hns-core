/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_EXTENSIONS_API_SETTINGS_PRIVATE_HNS_PREFS_UTIL_H_
#define HNS_BROWSER_EXTENSIONS_API_SETTINGS_PRIVATE_HNS_PREFS_UTIL_H_

#include "chrome/browser/extensions/api/settings_private/prefs_util.h"

namespace extensions {

class HnsPrefsUtil : public PrefsUtil {
 public:
  using PrefsUtil::PrefsUtil;
  // Gets the list of allowlisted pref keys -- that is, those which correspond
  // to prefs that clients of the settingsPrivate API may retrieve and
  // manipulate.
  const PrefsUtil::TypedPrefMap& GetAllowlistedKeys() override;
};

}  // namespace extensions

#endif  // HNS_BROWSER_EXTENSIONS_API_SETTINGS_PRIVATE_HNS_PREFS_UTIL_H_
