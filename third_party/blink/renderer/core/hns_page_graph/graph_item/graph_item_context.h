/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_THIRD_PARTY_BLINK_RENDERER_CORE_HNS_PAGE_GRAPH_GRAPH_ITEM_GRAPH_ITEM_CONTEXT_H_
#define HNS_THIRD_PARTY_BLINK_RENDERER_CORE_HNS_PAGE_GRAPH_GRAPH_ITEM_GRAPH_ITEM_CONTEXT_H_

#include "base/time/time.h"
#include "hns/third_party/blink/renderer/core/hns_page_graph/types.h"

namespace hns_page_graph {

class GraphItemContext {
 public:
  virtual ~GraphItemContext() = default;

  virtual base::TimeTicks GetGraphStartTime() const = 0;
  virtual GraphItemId GetNextGraphItemId() = 0;
};

}  // namespace hns_page_graph

#endif  // HNS_THIRD_PARTY_BLINK_RENDERER_CORE_HNS_PAGE_GRAPH_GRAPH_ITEM_GRAPH_ITEM_CONTEXT_H_
