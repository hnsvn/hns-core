/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_CHROMIUM_SRC_THIRD_PARTY_BLINK_RENDERER_PLATFORM_LOADER_FETCH_FETCH_INITIATOR_INFO_H_
#define HNS_CHROMIUM_SRC_THIRD_PARTY_BLINK_RENDERER_PLATFORM_LOADER_FETCH_FETCH_INITIATOR_INFO_H_

#include "hns/components/hns_page_graph/common/buildflags.h"
#include "third_party/blink/renderer/platform/graphics/dom_node_id.h"

#if BUILDFLAG(ENABLE_HNS_PAGE_GRAPH)
#define referrer                             \
  referrer;                                  \
  DOMNodeId dom_node_id = kInvalidDOMNodeId; \
  int parent_script_id = 0
#endif  // BUILDFLAG(ENABLE_HNS_PAGE_GRAPH)

#include "src/third_party/blink/renderer/platform/loader/fetch/fetch_initiator_info.h"  // IWYU pragma: export

#if BUILDFLAG(ENABLE_HNS_PAGE_GRAPH)
#undef referrer
#endif  // BUILDFLAG(ENABLE_HNS_PAGE_GRAPH)

#endif  // HNS_CHROMIUM_SRC_THIRD_PARTY_BLINK_RENDERER_PLATFORM_LOADER_FETCH_FETCH_INITIATOR_INFO_H_
