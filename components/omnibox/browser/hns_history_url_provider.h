/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_OMNIBOX_BROWSER_HNS_HISTORY_URL_PROVIDER_H_
#define HNS_COMPONENTS_OMNIBOX_BROWSER_HNS_HISTORY_URL_PROVIDER_H_

#include "components/omnibox/browser/history_url_provider.h"

class HnsHistoryURLProvider : public HistoryURLProvider {
 public:
  using HistoryURLProvider::HistoryURLProvider;
  HnsHistoryURLProvider(const HnsHistoryURLProvider&) = delete;
  HnsHistoryURLProvider& operator=(const HnsHistoryURLProvider&) = delete;

  void Start(const AutocompleteInput& input, bool minimal_changes) override;

 private:
  ~HnsHistoryURLProvider() override;
};

#endif  // HNS_COMPONENTS_OMNIBOX_BROWSER_HNS_HISTORY_URL_PROVIDER_H_
