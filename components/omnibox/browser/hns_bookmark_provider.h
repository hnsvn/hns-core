/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_OMNIBOX_BROWSER_HNS_BOOKMARK_PROVIDER_H_
#define HNS_COMPONENTS_OMNIBOX_BROWSER_HNS_BOOKMARK_PROVIDER_H_

#include "components/omnibox/browser/bookmark_provider.h"

class HnsBookmarkProvider : public BookmarkProvider {
 public:
  using BookmarkProvider::BookmarkProvider;
  HnsBookmarkProvider(const HnsBookmarkProvider&) = delete;
  HnsBookmarkProvider& operator=(const HnsBookmarkProvider&) = delete;

  void Start(const AutocompleteInput& input, bool minimal_changes) override;

 private:
  ~HnsBookmarkProvider() override;
};

#endif  // HNS_COMPONENTS_OMNIBOX_BROWSER_HNS_BOOKMARK_PROVIDER_H_
