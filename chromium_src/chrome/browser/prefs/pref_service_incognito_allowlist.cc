/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/browser/prefs/hns_pref_service_incognito_allowlist.h"

#include "hns/components/constants/pref_names.h"
#include "components/bookmarks/common/bookmark_pref_names.h"

#define GetIncognitoPersistentPrefsAllowlist \
  GetIncognitoPersistentPrefsAllowlist_ChromiumImpl
#define kShowBookmarkBar kShowBookmarkBar, kAlwaysShowBookmarkBarOnNTP
#include "src/chrome/browser/prefs/pref_service_incognito_allowlist.cc"
#undef kShowBookmarkBar
#undef GetIncognitoPersistentPrefsAllowlist

namespace prefs {

std::vector<const char*> GetIncognitoPersistentPrefsAllowlist() {
  std::vector<const char*> allowlist =
      GetIncognitoPersistentPrefsAllowlist_ChromiumImpl();
  allowlist.insert(allowlist.end(),
                   hns::GetHnsPersistentPrefNames().begin(),
                   hns::GetHnsPersistentPrefNames().end());
  return allowlist;
}

}  // namespace prefs
