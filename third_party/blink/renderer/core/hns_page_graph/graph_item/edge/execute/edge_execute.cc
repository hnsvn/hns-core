/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/third_party/blink/renderer/core/hns_page_graph/graph_item/edge/execute/edge_execute.h"

#include "hns/third_party/blink/renderer/core/hns_page_graph/graph_item/node/actor/node_actor.h"
#include "hns/third_party/blink/renderer/core/hns_page_graph/graph_item/node/actor/node_script.h"
#include "hns/third_party/blink/renderer/core/hns_page_graph/graph_item/node/html/node_html_element.h"
#include "hns/third_party/blink/renderer/core/hns_page_graph/graph_item/node/node_extensions.h"

namespace hns_page_graph {

EdgeExecute::EdgeExecute(GraphItemContext* context,
                         NodeHTMLElement* out_node,
                         NodeScript* in_node)
    : GraphEdge(context, out_node, in_node) {}

EdgeExecute::EdgeExecute(GraphItemContext* context,
                         NodeExtensions* out_node,
                         NodeScript* in_node)
    : GraphEdge(context, out_node, in_node) {}

EdgeExecute::EdgeExecute(GraphItemContext* context,
                         NodeActor* out_node,
                         NodeScript* in_node)
    : GraphEdge(context, out_node, in_node) {}

EdgeExecute::~EdgeExecute() = default;

ItemName EdgeExecute::GetItemName() const {
  return "execute";
}

bool EdgeExecute::IsEdgeExecute() const {
  return true;
}

bool EdgeExecute::IsEdgeExecuteAttr() const {
  return false;
}

}  // namespace hns_page_graph
