/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_THIRD_PARTY_BLINK_RENDERER_CORE_HNS_PAGE_GRAPH_UTILITIES_URLS_H_
#define HNS_THIRD_PARTY_BLINK_RENDERER_CORE_HNS_PAGE_GRAPH_UTILITIES_URLS_H_

#include "hns/third_party/blink/renderer/core/hns_page_graph/types.h"

namespace hns_page_graph {

blink::KURL NormalizeUrl(const blink::KURL& url);

}  // namespace hns_page_graph

#endif  // HNS_THIRD_PARTY_BLINK_RENDERER_CORE_HNS_PAGE_GRAPH_UTILITIES_URLS_H_
