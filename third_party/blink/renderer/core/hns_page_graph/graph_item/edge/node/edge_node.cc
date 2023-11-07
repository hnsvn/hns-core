/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/third_party/blink/renderer/core/hns_page_graph/graph_item/edge/node/edge_node.h"

#include "hns/third_party/blink/renderer/core/hns_page_graph/graph_item/node/actor/node_actor.h"
#include "hns/third_party/blink/renderer/core/hns_page_graph/graph_item/node/html/node_html.h"

namespace hns_page_graph {

EdgeNode::EdgeNode(GraphItemContext* context,
                   NodeActor* out_node,
                   NodeHTML* in_node)
    : GraphEdge(context, out_node, in_node) {}

EdgeNode::~EdgeNode() = default;

bool EdgeNode::IsEdgeNode() const {
  return true;
}

bool EdgeNode::IsEdgeNodeCreate() const {
  return false;
}

bool EdgeNode::IsEdgeNodeDelete() const {
  return false;
}

bool EdgeNode::IsEdgeNodeInsert() const {
  return false;
}

bool EdgeNode::IsEdgeNodeRemove() const {
  return false;
}

}  // namespace hns_page_graph
