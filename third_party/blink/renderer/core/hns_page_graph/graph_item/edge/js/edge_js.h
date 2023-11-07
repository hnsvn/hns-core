/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_THIRD_PARTY_BLINK_RENDERER_CORE_HNS_PAGE_GRAPH_GRAPH_ITEM_EDGE_JS_EDGE_JS_H_
#define HNS_THIRD_PARTY_BLINK_RENDERER_CORE_HNS_PAGE_GRAPH_GRAPH_ITEM_EDGE_JS_EDGE_JS_H_

#include "hns/third_party/blink/renderer/core/hns_page_graph/graph_item/edge/graph_edge.h"
#include "third_party/blink/renderer/platform/wtf/casting.h"

namespace hns_page_graph {

class GraphNode;

class EdgeJS : public GraphEdge {
 public:
  EdgeJS(GraphItemContext* context, GraphNode* out_node, GraphNode* in_node);
  ~EdgeJS() override;

  void AddGraphMLAttributes(xmlDocPtr doc,
                            xmlNodePtr parent_node) const override;

  virtual const MethodName& GetMethodName() const = 0;
  bool IsEdgeJS() const override;
  virtual bool IsEdgeJSCall() const;
  virtual bool IsEdgeJSResult() const;
};

}  // namespace hns_page_graph

namespace blink {

template <>
struct DowncastTraits<hns_page_graph::EdgeJS> {
  static bool AllowFrom(const hns_page_graph::GraphEdge& edge) {
    return edge.IsEdgeJS();
  }
  static bool AllowFrom(const hns_page_graph::GraphItem& graph_item) {
    return IsA<hns_page_graph::EdgeJS>(
        DynamicTo<hns_page_graph::GraphEdge>(graph_item));
  }
};

}  // namespace blink

#endif  // HNS_THIRD_PARTY_BLINK_RENDERER_CORE_HNS_PAGE_GRAPH_GRAPH_ITEM_EDGE_JS_EDGE_JS_H_
