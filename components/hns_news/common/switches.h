// Copyright (c) 2020 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef HNS_COMPONENTS_HNS_NEWS_COMMON_SWITCHES_H_
#define HNS_COMPONENTS_HNS_NEWS_COMMON_SWITCHES_H_

namespace hns_news {
namespace switches {

// Allow providing an alternate host for Hns News feeds (e.g. staging or dev)
extern const char kHnsNewsHost[];

// Allow overriding the region for Hns News feeds (e.g. ja, en_US ect.)
extern const char kHnsNewsRegion[];

}  // namespace switches
}  // namespace hns_news

#endif  // HNS_COMPONENTS_HNS_NEWS_COMMON_SWITCHES_H_
