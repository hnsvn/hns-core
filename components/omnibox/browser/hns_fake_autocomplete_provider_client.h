/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_OMNIBOX_BROWSER_HNS_FAKE_AUTOCOMPLETE_PROVIDER_CLIENT_H_
#define HNS_COMPONENTS_OMNIBOX_BROWSER_HNS_FAKE_AUTOCOMPLETE_PROVIDER_CLIENT_H_

#include <memory>

#include "base/memory/scoped_refptr.h"
#include "components/omnibox/browser/mock_autocomplete_provider_client.h"
#include "components/omnibox/browser/shortcuts_backend.h"
#include "components/prefs/testing_pref_service.h"

class HnsFakeAutocompleteProviderClient
    : public MockAutocompleteProviderClient {
 public:
  HnsFakeAutocompleteProviderClient();
  HnsFakeAutocompleteProviderClient(
      const HnsFakeAutocompleteProviderClient&) = delete;
  HnsFakeAutocompleteProviderClient& operator=(
      const HnsFakeAutocompleteProviderClient&) = delete;
  ~HnsFakeAutocompleteProviderClient() override;
  PrefService* GetPrefs() const override;

  void set_shortcuts_backend(
      scoped_refptr<ShortcutsBackend> shortcuts_backend) {
    shortcuts_backend_ = shortcuts_backend;
  }

  scoped_refptr<ShortcutsBackend> GetShortcutsBackend() override;
  scoped_refptr<ShortcutsBackend> GetShortcutsBackendIfExists() override;

 private:
  std::unique_ptr<TestingPrefServiceSimple> pref_service_;
  scoped_refptr<ShortcutsBackend> shortcuts_backend_;
};

#endif  // HNS_COMPONENTS_OMNIBOX_BROWSER_HNS_FAKE_AUTOCOMPLETE_PROVIDER_CLIENT_H_
