/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_CHROMIUM_SRC_BASE_TRACE_EVENT_BUILTIN_CATEGORIES_H_
#define HNS_CHROMIUM_SRC_BASE_TRACE_EVENT_BUILTIN_CATEGORIES_H_

// Please add all hns categories here. Take a look at the original
// builtin_categories.h for more details. Please:
// * For small features use the existing 'hns' category.
// * For new big subsystems with a lot of traces create a separate category
// named 'hns.<feature_name>'.
// * Keep the list sorted.
//
// Note: There is no #undef for this macros because it is used in exported
// macros of the chromium builtin_categories.h.
#define HNS_INTERNAL_TRACE_LIST_BUILTIN_CATEGORIES(X) \
  X("hns")                                            \
  X("hns.adblock")

#include "src/base/trace_event/builtin_categories.h"  // IWYU pragma: export

#endif  // HNS_CHROMIUM_SRC_BASE_TRACE_EVENT_BUILTIN_CATEGORIES_H_
