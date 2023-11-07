/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/components/omnibox/browser/hns_fake_autocomplete_provider_client.h"

#include "base/memory/scoped_refptr.h"
#include "hns/components/omnibox/browser/hns_omnibox_prefs.h"
#include "components/omnibox/browser/shortcuts_backend.h"
#include "components/prefs/pref_registry_simple.h"

HnsFakeAutocompleteProviderClient::HnsFakeAutocompleteProviderClient() {
  pref_service_ = std::make_unique<TestingPrefServiceSimple>();
  omnibox::RegisterHnsProfilePrefs(pref_service_->registry());
}

HnsFakeAutocompleteProviderClient::~HnsFakeAutocompleteProviderClient() =
    default;

PrefService* HnsFakeAutocompleteProviderClient::GetPrefs() const {
  return pref_service_.get();
}

scoped_refptr<ShortcutsBackend>
HnsFakeAutocompleteProviderClient::GetShortcutsBackend() {
  return shortcuts_backend_;
}

scoped_refptr<ShortcutsBackend>
HnsFakeAutocompleteProviderClient::GetShortcutsBackendIfExists() {
  return shortcuts_backend_;
}
