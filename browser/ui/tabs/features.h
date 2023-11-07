/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_UI_TABS_FEATURES_H_
#define HNS_BROWSER_UI_TABS_FEATURES_H_

#include "base/feature_list.h"
namespace tabs::features {

BASE_DECLARE_FEATURE(kHnsVerticalTabs);
BASE_DECLARE_FEATURE(kHnsVerticalTabsStickyPinnedTabs);

#if BUILDFLAG(IS_LINUX)
// This flag controls the behavior of browser_default::kScrollEventChangesTab,
// which is true only when it's Linux.
BASE_DECLARE_FEATURE(kHnsChangeActiveTabOnScrollEvent);
#endif  // BUILDFLAG(IS_LINUX)

BASE_DECLARE_FEATURE(kHnsSharedPinnedTabs);

}  // namespace tabs::features

#endif  // HNS_BROWSER_UI_TABS_FEATURES_H_
