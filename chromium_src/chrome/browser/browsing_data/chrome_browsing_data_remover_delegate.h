/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_CHROMIUM_SRC_CHROME_BROWSER_BROWSING_DATA_CHROME_BROWSING_DATA_REMOVER_DELEGATE_H_
#define HNS_CHROMIUM_SRC_CHROME_BROWSER_BROWSING_DATA_CHROME_BROWSING_DATA_REMOVER_DELEGATE_H_

class HnsBrowsingDataRemoverDelegate;

#define HNS_CHROME_BROWSING_DATA_REMOVER_DELEGATE_H \
  friend class HnsBrowsingDataRemoverDelegate;

#include "src/chrome/browser/browsing_data/chrome_browsing_data_remover_delegate.h"  // IWYU pragma: export
#undef HNS_CHROME_BROWSING_DATA_REMOVER_DELEGATE_H

#endif  // HNS_CHROMIUM_SRC_CHROME_BROWSER_BROWSING_DATA_CHROME_BROWSING_DATA_REMOVER_DELEGATE_H_
