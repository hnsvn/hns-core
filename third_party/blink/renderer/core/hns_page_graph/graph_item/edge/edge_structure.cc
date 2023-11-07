/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/third_party/blink/renderer/core/hns_page_graph/graph_item/edge/edge_structure.h"

#include "hns/third_party/blink/renderer/core/hns_page_graph/graph_item/edge/graph_edge.h"
#include "hns/third_party/blink/renderer/core/hns_page_graph/graph_item/node/html/node_html.h"
#include "hns/third_party/blink/renderer/core/hns_page_graph/graph_item/node/html/node_html_element.h"

namespace hns_page_graph {

EdgeStructure::EdgeStructure(GraphItemContext* context,
                             GraphNode* out_node,
                             GraphNode* in_node)
    : GraphEdge(context, out_node, in_node) {}

EdgeStructure::~EdgeStructure() = default;

ItemName EdgeStructure::GetItemName() const {
  return "structure";
}

bool EdgeStructure::IsEdgeStructure() const {
  return true;
}

}  // namespace hns_page_graph
