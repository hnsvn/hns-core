/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/components/omnibox/browser/hns_history_url_provider.h"

#include "hns/components/omnibox/browser/hns_omnibox_prefs.h"
#include "components/history/core/browser/history_service.h"
#include "components/prefs/pref_service.h"

HnsHistoryURLProvider::~HnsHistoryURLProvider() = default;

void HnsHistoryURLProvider::Start(const AutocompleteInput& input,
                                    bool minimal_changes) {
  // Unlike other providers, we can't simply stop the search here. The
  // HistoryURLProvider doesn't only search history, it is also responsible for
  // navigating to exact urls (i.e. https://example.com/), so we need to disable
  // **ONLY** history searches. Fortunately, Chromium has a flag for this.
  search_url_database_ =
      client()->GetPrefs()->GetBoolean(omnibox::kHistorySuggestionsEnabled);

  HistoryURLProvider::Start(input, minimal_changes);
}
