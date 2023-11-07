/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_THIRD_PARTY_BLINK_RENDERER_CORE_HNS_PAGE_GRAPH_GRAPH_ITEM_EDGE_NODE_EDGE_NODE_INSERT_H_
#define HNS_THIRD_PARTY_BLINK_RENDERER_CORE_HNS_PAGE_GRAPH_GRAPH_ITEM_EDGE_NODE_EDGE_NODE_INSERT_H_

#include "hns/third_party/blink/renderer/core/hns_page_graph/graph_item/edge/node/edge_node.h"
#include "third_party/blink/renderer/platform/wtf/casting.h"

namespace hns_page_graph {

class NodeActor;
class NodeHTML;
class NodeHTMLElement;

class EdgeNodeInsert final : public EdgeNode {
 public:
  EdgeNodeInsert(GraphItemContext* context,
                 NodeActor* out_node,
                 NodeHTML* in_node,
                 NodeHTMLElement* parent_node = nullptr,
                 NodeHTML* prior_sibling_node = nullptr);

  ~EdgeNodeInsert() override;

  NodeHTMLElement* GetParentNode() const;
  NodeHTML* GetPriorSiblingNode() const;

  ItemName GetItemName() const override;
  ItemDesc GetItemDesc() const override;

  void AddGraphMLAttributes(xmlDocPtr doc,
                            xmlNodePtr parent_node) const override;

  bool IsEdgeNodeInsert() const override;

 private:
  NodeHTMLElement* parent_node_;
  NodeHTML* prior_sibling_node_;
};

}  // namespace hns_page_graph

namespace blink {

template <>
struct DowncastTraits<hns_page_graph::EdgeNodeInsert> {
  static bool AllowFrom(const hns_page_graph::EdgeNode& edge) {
    return edge.IsEdgeNodeInsert();
  }
  static bool AllowFrom(const hns_page_graph::GraphEdge& edge) {
    return IsA<hns_page_graph::EdgeNodeInsert>(
        DynamicTo<hns_page_graph::EdgeNode>(edge));
  }
  static bool AllowFrom(const hns_page_graph::GraphItem& graph_item) {
    return IsA<hns_page_graph::EdgeNodeInsert>(
        DynamicTo<hns_page_graph::GraphEdge>(graph_item));
  }
};

}  // namespace blink

#endif  // HNS_THIRD_PARTY_BLINK_RENDERER_CORE_HNS_PAGE_GRAPH_GRAPH_ITEM_EDGE_NODE_EDGE_NODE_INSERT_H_
