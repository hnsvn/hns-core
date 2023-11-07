// Copyright (c) 2023 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef HNS_CHROMIUM_SRC_COMPONENTS_OMNIBOX_BROWSER_AUTOCOMPLETE_PROVIDER_H_
#define HNS_CHROMIUM_SRC_COMPONENTS_OMNIBOX_BROWSER_AUTOCOMPLETE_PROVIDER_H_

// Note: We go negative with the HnsAutoCompleteTypes, so we don't conflict if
// Chromium adds something new.
#define TYPE_BOOKMARK TYPE_HNS_COMMANDER = -1 << 0, TYPE_BOOKMARK
#include "src/components/omnibox/browser/autocomplete_provider.h"  // IWYU pragma: export
#undef TYPE_BOOKMARK

#endif  // HNS_CHROMIUM_SRC_COMPONENTS_OMNIBOX_BROWSER_AUTOCOMPLETE_PROVIDER_H_
