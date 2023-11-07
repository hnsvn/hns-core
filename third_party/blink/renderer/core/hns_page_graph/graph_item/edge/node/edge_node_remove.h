/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_THIRD_PARTY_BLINK_RENDERER_CORE_HNS_PAGE_GRAPH_GRAPH_ITEM_EDGE_NODE_EDGE_NODE_REMOVE_H_
#define HNS_THIRD_PARTY_BLINK_RENDERER_CORE_HNS_PAGE_GRAPH_GRAPH_ITEM_EDGE_NODE_EDGE_NODE_REMOVE_H_

#include "hns/third_party/blink/renderer/core/hns_page_graph/graph_item/edge/node/edge_node.h"
#include "third_party/blink/renderer/platform/wtf/casting.h"

namespace hns_page_graph {

class NodeHTML;
class NodeScript;

class EdgeNodeRemove final : public EdgeNode {
 public:
  EdgeNodeRemove(GraphItemContext* context,
                 NodeScript* out_node,
                 NodeHTML* in_node);
  ~EdgeNodeRemove() override;

  ItemName GetItemName() const override;

  bool IsEdgeNodeRemove() const override;
};

}  // namespace hns_page_graph

namespace blink {

template <>
struct DowncastTraits<hns_page_graph::EdgeNodeRemove> {
  static bool AllowFrom(const hns_page_graph::EdgeNode& edge) {
    return edge.IsEdgeNodeRemove();
  }
  static bool AllowFrom(const hns_page_graph::GraphEdge& edge) {
    return IsA<hns_page_graph::EdgeNodeRemove>(
        DynamicTo<hns_page_graph::EdgeNode>(edge));
  }
  static bool AllowFrom(const hns_page_graph::GraphItem& graph_item) {
    return IsA<hns_page_graph::EdgeNodeRemove>(
        DynamicTo<hns_page_graph::GraphEdge>(graph_item));
  }
};

}  // namespace blink

#endif  // HNS_THIRD_PARTY_BLINK_RENDERER_CORE_HNS_PAGE_GRAPH_GRAPH_ITEM_EDGE_NODE_EDGE_NODE_REMOVE_H_
