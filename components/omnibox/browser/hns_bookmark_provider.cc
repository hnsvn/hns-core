/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/components/omnibox/browser/hns_bookmark_provider.h"

#include "hns/components/omnibox/browser/hns_history_quick_provider.h"
#include "hns/components/omnibox/browser/hns_omnibox_prefs.h"
#include "components/omnibox/browser/bookmark_provider.h"
#include "components/prefs/pref_service.h"

HnsBookmarkProvider::~HnsBookmarkProvider() = default;

void HnsBookmarkProvider::Start(const AutocompleteInput& input,
                                  bool minimal_changes) {
  if (!client_->GetPrefs()->GetBoolean(omnibox::kBookmarkSuggestionsEnabled)) {
    matches_.clear();
    return;
  }
  BookmarkProvider::Start(input, minimal_changes);
}
