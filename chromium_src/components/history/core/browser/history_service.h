/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_CHROMIUM_SRC_COMPONENTS_HISTORY_CORE_BROWSER_HISTORY_SERVICE_H_
#define HNS_CHROMIUM_SRC_COMPONENTS_HISTORY_CORE_BROWSER_HISTORY_SERVICE_H_

class HnsHistoryURLProviderTest;
class HnsHistoryQuickProviderTest;

#define Cleanup                                 \
  Cleanup();                                    \
  friend class ::HnsHistoryURLProviderTest;   \
  friend class ::HnsHistoryQuickProviderTest; \
  void CleanupUnused

#include "src/components/history/core/browser/history_service.h"  // IWYU pragma: export

#undef Cleanup

#endif  // HNS_CHROMIUM_SRC_COMPONENTS_HISTORY_CORE_BROWSER_HISTORY_SERVICE_H_
