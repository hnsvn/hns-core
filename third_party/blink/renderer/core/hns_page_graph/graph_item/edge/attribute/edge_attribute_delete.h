/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_THIRD_PARTY_BLINK_RENDERER_CORE_HNS_PAGE_GRAPH_GRAPH_ITEM_EDGE_ATTRIBUTE_EDGE_ATTRIBUTE_DELETE_H_
#define HNS_THIRD_PARTY_BLINK_RENDERER_CORE_HNS_PAGE_GRAPH_GRAPH_ITEM_EDGE_ATTRIBUTE_EDGE_ATTRIBUTE_DELETE_H_

#include "hns/third_party/blink/renderer/core/hns_page_graph/graph_item/edge/attribute/edge_attribute.h"
#include "third_party/blink/renderer/platform/wtf/casting.h"
#include "third_party/blink/renderer/platform/wtf/text/wtf_string.h"

namespace hns_page_graph {

class NodeActor;
class NodeHTMLElement;

class EdgeAttributeDelete final : public EdgeAttribute {
 public:
  EdgeAttributeDelete(GraphItemContext* context,
                      NodeActor* out_node,
                      NodeHTMLElement* in_node,
                      const String& name,
                      const bool is_style = false);
  ~EdgeAttributeDelete() override;

  ItemName GetItemName() const override;

  bool IsEdgeAttributeDelete() const override;
};

}  // namespace hns_page_graph

namespace blink {

template <>
struct DowncastTraits<hns_page_graph::EdgeAttributeDelete> {
  static bool AllowFrom(const hns_page_graph::EdgeAttribute& attribute_edge) {
    return attribute_edge.IsEdgeAttributeDelete();
  }
  static bool AllowFrom(const hns_page_graph::GraphEdge& edge) {
    return IsA<hns_page_graph::EdgeAttributeDelete>(
        DynamicTo<hns_page_graph::EdgeAttribute>(edge));
  }
  static bool AllowFrom(const hns_page_graph::GraphItem& graph_item) {
    return IsA<hns_page_graph::EdgeAttributeDelete>(
        DynamicTo<hns_page_graph::GraphEdge>(graph_item));
  }
};

}  // namespace blink

#endif  // HNS_THIRD_PARTY_BLINK_RENDERER_CORE_HNS_PAGE_GRAPH_GRAPH_ITEM_EDGE_ATTRIBUTE_EDGE_ATTRIBUTE_DELETE_H_
