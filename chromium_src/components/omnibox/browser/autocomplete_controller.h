/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_CHROMIUM_SRC_COMPONENTS_OMNIBOX_BROWSER_AUTOCOMPLETE_CONTROLLER_H_
#define HNS_CHROMIUM_SRC_COMPONENTS_OMNIBOX_BROWSER_AUTOCOMPLETE_CONTROLLER_H_

#define AutocompleteControllerTest \
  OmniboxPromotionTest;            \
  friend class AutocompleteControllerTest

#include "src/components/omnibox/browser/autocomplete_controller.h"  // IWYU pragma: export

#undef AutocompleteControllerTest

#endif  // HNS_CHROMIUM_SRC_COMPONENTS_OMNIBOX_BROWSER_AUTOCOMPLETE_CONTROLLER_H_
