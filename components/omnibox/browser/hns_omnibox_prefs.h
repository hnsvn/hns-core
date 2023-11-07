/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_OMNIBOX_BROWSER_HNS_OMNIBOX_PREFS_H_
#define HNS_COMPONENTS_OMNIBOX_BROWSER_HNS_OMNIBOX_PREFS_H_

class PrefRegistrySimple;

namespace omnibox {

extern const char kAutocompleteEnabled[];
extern const char kTopSiteSuggestionsEnabled[];
extern const char kHistorySuggestionsEnabled[];
extern const char kBookmarkSuggestionsEnabled[];

void RegisterHnsProfilePrefs(PrefRegistrySimple* registry);

}  // namespace omnibox

#endif  // HNS_COMPONENTS_OMNIBOX_BROWSER_HNS_OMNIBOX_PREFS_H_
