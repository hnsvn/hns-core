/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/third_party/blink/renderer/core/hns_page_graph/graph_item/edge/js/edge_js.h"

#include "hns/third_party/blink/renderer/core/hns_page_graph/graphml.h"

namespace hns_page_graph {

EdgeJS::EdgeJS(GraphItemContext* context,
               GraphNode* out_node,
               GraphNode* in_node)
    : GraphEdge(context, out_node, in_node) {}

EdgeJS::~EdgeJS() = default;

void EdgeJS::AddGraphMLAttributes(xmlDocPtr doc, xmlNodePtr parent_node) const {
  GraphEdge::AddGraphMLAttributes(doc, parent_node);
}

bool EdgeJS::IsEdgeJS() const {
  return true;
}

bool EdgeJS::IsEdgeJSCall() const {
  return false;
}

bool EdgeJS::IsEdgeJSResult() const {
  return false;
}

}  // namespace hns_page_graph
