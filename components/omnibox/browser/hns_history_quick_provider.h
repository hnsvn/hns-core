/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_OMNIBOX_BROWSER_HNS_HISTORY_QUICK_PROVIDER_H_
#define HNS_COMPONENTS_OMNIBOX_BROWSER_HNS_HISTORY_QUICK_PROVIDER_H_

#include "components/omnibox/browser/history_quick_provider.h"

class HnsHistoryQuickProvider : public HistoryQuickProvider {
 public:
  using HistoryQuickProvider::HistoryQuickProvider;
  HnsHistoryQuickProvider(const HnsHistoryQuickProvider&) = delete;
  HnsHistoryQuickProvider& operator=(const HnsHistoryQuickProvider&) =
      delete;

  void Start(const AutocompleteInput& input, bool minimal_changes) override;

 private:
  ~HnsHistoryQuickProvider() override;
};

#endif  // HNS_COMPONENTS_OMNIBOX_BROWSER_HNS_HISTORY_QUICK_PROVIDER_H_
