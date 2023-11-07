/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_CHROMIUM_SRC_COMPONENTS_SEARCH_ENGINES_SEARCH_ENGINES_PREF_NAMES_H_
#define HNS_CHROMIUM_SRC_COMPONENTS_SEARCH_ENGINES_SEARCH_ENGINES_PREF_NAMES_H_

#include "src/components/search_engines/search_engines_pref_names.h"  // IWYU pragma: export

namespace prefs {

extern const char kDefaultSearchProviderByExtension[];
extern const char kAddOpenSearchEngines[];
extern const char kHnsDefaultSearchVersion[];
extern const char kSyncedDefaultPrivateSearchProviderGUID[];
extern const char kSyncedDefaultPrivateSearchProviderData[];

}  // namespace prefs

#endif  // HNS_CHROMIUM_SRC_COMPONENTS_SEARCH_ENGINES_SEARCH_ENGINES_PREF_NAMES_H_
