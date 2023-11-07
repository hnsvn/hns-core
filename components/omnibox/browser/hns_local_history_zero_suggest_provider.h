// Copyright (c) 2023 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef HNS_COMPONENTS_OMNIBOX_BROWSER_HNS_LOCAL_HISTORY_ZERO_SUGGEST_PROVIDER_H_
#define HNS_COMPONENTS_OMNIBOX_BROWSER_HNS_LOCAL_HISTORY_ZERO_SUGGEST_PROVIDER_H_

#include "components/omnibox/browser/local_history_zero_suggest_provider.h"

class HnsLocalHistoryZeroSuggestProvider
    : public LocalHistoryZeroSuggestProvider {
 public:
  static HnsLocalHistoryZeroSuggestProvider* Create(
      AutocompleteProviderClient* client,
      AutocompleteProviderListener* listener);

  using LocalHistoryZeroSuggestProvider::LocalHistoryZeroSuggestProvider;

  HnsLocalHistoryZeroSuggestProvider(
      const HnsLocalHistoryZeroSuggestProvider&) = delete;
  HnsLocalHistoryZeroSuggestProvider& operator=(
      const HnsLocalHistoryZeroSuggestProvider&) = delete;

  void Start(const AutocompleteInput& input, bool minimal_changes) override;

 private:
  ~HnsLocalHistoryZeroSuggestProvider() override;
};

#endif  // HNS_COMPONENTS_OMNIBOX_BROWSER_HNS_LOCAL_HISTORY_ZERO_SUGGEST_PROVIDER_H_
