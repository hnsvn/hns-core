/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/browser/ui/tabs/features.h"

namespace tabs::features {

BASE_FEATURE(kHnsVerticalTabs,
             "HnsVerticalTabs",
             base::FEATURE_ENABLED_BY_DEFAULT);

BASE_FEATURE(kHnsVerticalTabsStickyPinnedTabs,
             "HnsVerticalTabsStickyPinnedTabs",
             base::FEATURE_ENABLED_BY_DEFAULT);

#if BUILDFLAG(IS_LINUX)
BASE_FEATURE(kHnsChangeActiveTabOnScrollEvent,
             "HnsChangeActiveTabOnScrollEvent",
             base::FEATURE_ENABLED_BY_DEFAULT);
#endif  // BUILDFLAG(IS_LINUX)

BASE_FEATURE(kHnsSharedPinnedTabs,
             "HnsSharedPinnedTabs",
             base::FEATURE_DISABLED_BY_DEFAULT);

}  // namespace tabs::features
