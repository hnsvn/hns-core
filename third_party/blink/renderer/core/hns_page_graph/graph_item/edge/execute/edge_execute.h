/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_THIRD_PARTY_BLINK_RENDERER_CORE_HNS_PAGE_GRAPH_GRAPH_ITEM_EDGE_EXECUTE_EDGE_EXECUTE_H_
#define HNS_THIRD_PARTY_BLINK_RENDERER_CORE_HNS_PAGE_GRAPH_GRAPH_ITEM_EDGE_EXECUTE_EDGE_EXECUTE_H_

#include "hns/third_party/blink/renderer/core/hns_page_graph/graph_item/edge/graph_edge.h"
#include "third_party/blink/renderer/platform/wtf/casting.h"

namespace hns_page_graph {

class NodeExtensions;
class NodeHTMLElement;
class NodeActor;
class NodeScript;

class EdgeExecute : public GraphEdge {
 public:
  EdgeExecute(GraphItemContext* context,
              NodeHTMLElement* out_node,
              NodeScript* in_node);
  EdgeExecute(GraphItemContext* context,
              NodeExtensions* out_node,
              NodeScript* in_node);
  // Used by imported module scripts and eval
  EdgeExecute(GraphItemContext* context,
              NodeActor* out_node,
              NodeScript* in_node);
  ~EdgeExecute() override;

  ItemName GetItemName() const override;

  bool IsEdgeExecute() const override;

  virtual bool IsEdgeExecuteAttr() const;
};

}  // namespace hns_page_graph

namespace blink {

template <>
struct DowncastTraits<hns_page_graph::EdgeExecute> {
  static bool AllowFrom(const hns_page_graph::GraphEdge& edge) {
    return edge.IsEdgeExecute();
  }
  static bool AllowFrom(const hns_page_graph::GraphItem& graph_item) {
    return IsA<hns_page_graph::EdgeExecute>(
        DynamicTo<hns_page_graph::GraphEdge>(graph_item));
  }
};

}  // namespace blink

#endif  // HNS_THIRD_PARTY_BLINK_RENDERER_CORE_HNS_PAGE_GRAPH_GRAPH_ITEM_EDGE_EXECUTE_EDGE_EXECUTE_H_
