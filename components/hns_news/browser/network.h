// Copyright (c) 2021 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.
#ifndef HNS_COMPONENTS_HNS_NEWS_BROWSER_NETWORK_H_
#define HNS_COMPONENTS_HNS_NEWS_BROWSER_NETWORK_H_

#include "net/traffic_annotation/network_traffic_annotation.h"

namespace hns_news {

net::NetworkTrafficAnnotationTag GetNetworkTrafficAnnotationTag();

}  // namespace hns_news

#endif  // HNS_COMPONENTS_HNS_NEWS_BROWSER_NETWORK_H_
