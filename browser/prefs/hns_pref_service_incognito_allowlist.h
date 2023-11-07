/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_PREFS_HNS_PREF_SERVICE_INCOGNITO_ALLOWLIST_H_
#define HNS_BROWSER_PREFS_HNS_PREF_SERVICE_INCOGNITO_ALLOWLIST_H_

#include <vector>

namespace hns {

// Returns names of preferences that should be persistent on incognito profile.
// This list should be preferred over GetOriginalProfile().
const std::vector<const char*>& GetHnsPersistentPrefNames();

}  // namespace hns

#endif  // HNS_BROWSER_PREFS_HNS_PREF_SERVICE_INCOGNITO_ALLOWLIST_H_
