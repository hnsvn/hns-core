/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_THIRD_PARTY_BLINK_RENDERER_CORE_HNS_PAGE_GRAPH_GRAPH_ITEM_EDGE_EDGE_CROSS_DOM_H_
#define HNS_THIRD_PARTY_BLINK_RENDERER_CORE_HNS_PAGE_GRAPH_GRAPH_ITEM_EDGE_EDGE_CROSS_DOM_H_

#include "hns/third_party/blink/renderer/core/hns_page_graph/graph_item/edge/graph_edge.h"
#include "third_party/blink/renderer/platform/wtf/casting.h"

namespace hns_page_graph {

class NodeDOMRoot;
class NodeFrameOwner;
class NodeRemoteFrame;
class NodeActor;

class EdgeCrossDOM final : public GraphEdge {
 public:
  EdgeCrossDOM(GraphItemContext* context,
               NodeFrameOwner* out_node,
               NodeActor* in_node);
  EdgeCrossDOM(GraphItemContext* context,
               NodeDOMRoot* out_node,
               NodeDOMRoot* in_node);
  EdgeCrossDOM(GraphItemContext* context,
               NodeFrameOwner* out_node,
               NodeRemoteFrame* in_node);
  EdgeCrossDOM(GraphItemContext* context,
               NodeActor* out_node,
               NodeActor* in_node);
  ~EdgeCrossDOM() override;

  ItemName GetItemName() const override;

  bool IsEdgeCrossDOM() const override;
};

}  // namespace hns_page_graph

namespace blink {

template <>
struct DowncastTraits<hns_page_graph::EdgeCrossDOM> {
  static bool AllowFrom(const hns_page_graph::GraphEdge& edge) {
    return edge.IsEdgeCrossDOM();
  }
  static bool AllowFrom(const hns_page_graph::GraphItem& graph_item) {
    return IsA<hns_page_graph::EdgeCrossDOM>(
        DynamicTo<hns_page_graph::GraphEdge>(graph_item));
  }
};

}  // namespace blink

#endif  // HNS_THIRD_PARTY_BLINK_RENDERER_CORE_HNS_PAGE_GRAPH_GRAPH_ITEM_EDGE_EDGE_CROSS_DOM_H_
