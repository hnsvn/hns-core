/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_CHROMIUM_SRC_COMPONENTS_OMNIBOX_BROWSER_HISTORY_QUICK_PROVIDER_H_
#define HNS_CHROMIUM_SRC_COMPONENTS_OMNIBOX_BROWSER_HISTORY_QUICK_PROVIDER_H_

#define DoAutocomplete                    \
  DoAutocompleteUnused();                 \
  friend class HnsHistoryQuickProvider; \
  void DoAutocomplete

#include "src/components/omnibox/browser/history_quick_provider.h"  // IWYU pragma: export

#undef DoAutocomplete

#endif  // HNS_CHROMIUM_SRC_COMPONENTS_OMNIBOX_BROWSER_HISTORY_QUICK_PROVIDER_H_
