/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_OMNIBOX_BROWSER_HNS_SEARCH_PROVIDER_H_
#define HNS_COMPONENTS_OMNIBOX_BROWSER_HNS_SEARCH_PROVIDER_H_

#include "components/omnibox/browser/search_provider.h"

class HnsSearchProvider : public SearchProvider {
 public:
  using SearchProvider::SearchProvider;
  HnsSearchProvider(const HnsSearchProvider&) = delete;
  HnsSearchProvider& operator=(const HnsSearchProvider&) = delete;

  void DoHistoryQuery(bool minimal_changes) override;

 protected:
  ~HnsSearchProvider() override;
};

#endif  // HNS_COMPONENTS_OMNIBOX_BROWSER_HNS_SEARCH_PROVIDER_H_
