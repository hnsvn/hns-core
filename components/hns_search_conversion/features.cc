/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_search_conversion/features.h"

#include "base/feature_list.h"

namespace hns_search_conversion {

namespace features {

// Hns search promotion match located at last low in omnibox popup.
// This type seems more like ads banner.
BASE_FEATURE(kOmniboxBanner,
             "HnsSearchOmniboxBanner",
             base::FEATURE_DISABLED_BY_DEFAULT);

// Hns search promotion match located at second low in omnibox popup.
// This looks very similar with other normal matches.
BASE_FEATURE(kOmniboxButton,
             "HnsSearchOmniboxButton",
             base::FEATURE_DISABLED_BY_DEFAULT);

// Hns search promotion at NTP.
BASE_FEATURE(kNTP, "HnsSearchNTP", base::FEATURE_DISABLED_BY_DEFAULT);

}  // namespace features

}  // namespace hns_search_conversion
