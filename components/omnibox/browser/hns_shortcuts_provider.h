/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_OMNIBOX_BROWSER_HNS_SHORTCUTS_PROVIDER_H_
#define HNS_COMPONENTS_OMNIBOX_BROWSER_HNS_SHORTCUTS_PROVIDER_H_

#include "components/omnibox/browser/shortcuts_provider.h"

class HnsShortcutsProvider : public ShortcutsProvider {
 public:
  using ShortcutsProvider::ShortcutsProvider;

  HnsShortcutsProvider(const HnsShortcutsProvider&) = delete;
  HnsShortcutsProvider& operator=(const HnsShortcutsProvider&) = delete;

  void Start(const AutocompleteInput& input, bool minimal_changes) override;

 private:
  ~HnsShortcutsProvider() override;
};

#endif  // HNS_COMPONENTS_OMNIBOX_BROWSER_HNS_SHORTCUTS_PROVIDER_H_
