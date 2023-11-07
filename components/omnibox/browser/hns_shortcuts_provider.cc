/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/components/omnibox/browser/hns_shortcuts_provider.h"

#include "hns/components/omnibox/browser/hns_omnibox_prefs.h"
#include "components/omnibox/browser/autocomplete_provider_client.h"
#include "components/omnibox/browser/shortcuts_provider.h"
#include "components/prefs/pref_service.h"

HnsShortcutsProvider::~HnsShortcutsProvider() = default;

void HnsShortcutsProvider::Start(const AutocompleteInput& input,
                                   bool minimal_changes) {
  if (!client_->GetPrefs()->GetBoolean(omnibox::kHistorySuggestionsEnabled)) {
    matches_.clear();
    return;
  }
  ShortcutsProvider::Start(input, minimal_changes);
}
