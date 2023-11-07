// Copyright (c) 2021 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef HNS_COMPONENTS_HNS_SEARCH_COMMON_HNS_SEARCH_UTILS_H_
#define HNS_COMPONENTS_HNS_SEARCH_COMMON_HNS_SEARCH_UTILS_H_

class GURL;

namespace hns_search {

bool IsAllowedHost(const GURL& url);
bool IsDefaultAPIEnabled();

}  // namespace hns_search

#endif  // HNS_COMPONENTS_HNS_SEARCH_COMMON_HNS_SEARCH_UTILS_H_
