// Copyright (c) 2021 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef HNS_COMPONENTS_HNS_SEARCH_BROWSER_PREFS_H_
#define HNS_COMPONENTS_HNS_SEARCH_BROWSER_PREFS_H_

#include "build/build_config.h"

namespace hns_search {
namespace prefs {

constexpr char kDailyAsked[] = "hns.hns_search.daily_asked";
constexpr char kTotalAsked[] = "hns.hns_search.total_asked";
#if BUILDFLAG(IS_ANDROID)
constexpr char kFetchFromNative[] = "hns.hns_search.fetch_se_from_native";
#endif

}  // namespace prefs
}  // namespace hns_search

#endif  // HNS_COMPONENTS_HNS_SEARCH_BROWSER_PREFS_H_
