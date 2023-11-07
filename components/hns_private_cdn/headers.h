// Copyright (c) 2021 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef HNS_COMPONENTS_HNS_PRIVATE_CDN_HEADERS_H_
#define HNS_COMPONENTS_HNS_PRIVATE_CDN_HEADERS_H_

#include <string>

#include "base/containers/flat_map.h"

namespace hns {

const base::flat_map<std::string, std::string> private_cdn_headers = {
    {"User-Agent", ""},
    {"Accept-Language", ""}};

}  // namespace hns

#endif  // HNS_COMPONENTS_HNS_PRIVATE_CDN_HEADERS_H_
