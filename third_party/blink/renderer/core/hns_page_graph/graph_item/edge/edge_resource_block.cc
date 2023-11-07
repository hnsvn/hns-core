/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/third_party/blink/renderer/core/hns_page_graph/graph_item/edge/edge_resource_block.h"

#include "hns/third_party/blink/renderer/core/hns_page_graph/graph_item/node/filter/node_filter.h"
#include "hns/third_party/blink/renderer/core/hns_page_graph/graph_item/node/node_resource.h"
#include "hns/third_party/blink/renderer/core/hns_page_graph/graph_item/node/shield/node_shield.h"

namespace hns_page_graph {

EdgeResourceBlock::EdgeResourceBlock(GraphItemContext* context,
                                     NodeFilter* out_node,
                                     NodeResource* in_node)
    : GraphEdge(context, out_node, in_node) {}

EdgeResourceBlock::EdgeResourceBlock(GraphItemContext* context,
                                     NodeShield* out_node,
                                     NodeResource* in_node)
    : GraphEdge(context, out_node, in_node) {}

EdgeResourceBlock::~EdgeResourceBlock() = default;

ItemName EdgeResourceBlock::GetItemName() const {
  return "resource block";
}

bool EdgeResourceBlock::IsEdgeResourceBlock() const {
  return true;
}

}  // namespace hns_page_graph
