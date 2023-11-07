/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/components/omnibox/browser/hns_omnibox_prefs.h"

#include "components/prefs/pref_registry_simple.h"

namespace omnibox {

// Controls whether or not auto complete is enabled. If disabled, this overrides
// the individual prefs listed below.
const char kAutocompleteEnabled[] = "hns.autocomplete_enabled";

// Determines whether top sites show up in the omnibox results. See
// |TopsitesProvider|.
const char kTopSiteSuggestionsEnabled[] = "hns.top_site_suggestions_enabled";

// Determines whether history suggestions show up in the omnibox results. This
// includes:
// 1. Results from the |HistoryURLProvider|, which aren't "What-You-Typed".
// 2. Results from the |HistoryQuickProvider|.
// 3. Results from the |ShortcutsProvider|.
// 4. Results from the |SearchProvider|, which aren't "What-You-Typed".
const char kHistorySuggestionsEnabled[] =
    "hns.omnibox.history_suggestions_enabled";

// Determines whether bookmarks show up in the omnibox results. This controls
// whether or not we include results from the |BookmarkProvider|.
const char kBookmarkSuggestionsEnabled[] =
    "hns.omnibox.bookmark_suggestions_enabled";

void RegisterHnsProfilePrefs(PrefRegistrySimple* registry) {
  registry->RegisterBooleanPref(kAutocompleteEnabled, true);
  registry->RegisterBooleanPref(kTopSiteSuggestionsEnabled, true);
  registry->RegisterBooleanPref(kHistorySuggestionsEnabled, true);
  registry->RegisterBooleanPref(kBookmarkSuggestionsEnabled, true);
}

}  // namespace omnibox
