/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_THIRD_PARTY_BLINK_RENDERER_CORE_HNS_PAGE_GRAPH_GRAPH_ITEM_EDGE_REQUEST_EDGE_REQUEST_START_H_
#define HNS_THIRD_PARTY_BLINK_RENDERER_CORE_HNS_PAGE_GRAPH_GRAPH_ITEM_EDGE_REQUEST_EDGE_REQUEST_START_H_

#include "hns/third_party/blink/renderer/core/hns_page_graph/graph_item/edge/request/edge_request.h"
#include "third_party/blink/renderer/platform/wtf/casting.h"
#include "third_party/blink/renderer/platform/wtf/text/wtf_string.h"

namespace hns_page_graph {

class GraphNode;
class NodeResource;

class EdgeRequestStart final : public EdgeRequest {
 public:
  EdgeRequestStart(GraphItemContext* context,
                   GraphNode* out_node,
                   NodeResource* in_node,
                   const InspectorId request_id,
                   const String& resource_type);

  ~EdgeRequestStart() override;

  const String& GetResourceType() const { return resource_type_; }

  NodeResource* GetResourceNode() const override;
  GraphNode* GetRequestingNode() const override;

  ItemName GetItemName() const override;
  ItemDesc GetItemDesc() const override;

  void AddGraphMLAttributes(xmlDocPtr doc,
                            xmlNodePtr parent_node) const override;

  bool IsEdgeRequestStart() const override;

 private:
  const String resource_type_;
};

}  // namespace hns_page_graph

namespace blink {

template <>
struct DowncastTraits<hns_page_graph::EdgeRequestStart> {
  static bool AllowFrom(const hns_page_graph::EdgeRequest& request_edge) {
    return request_edge.IsEdgeRequestStart();
  }
  static bool AllowFrom(const hns_page_graph::GraphEdge& edge) {
    return IsA<hns_page_graph::EdgeRequestStart>(
        DynamicTo<hns_page_graph::EdgeRequest>(edge));
  }
  static bool AllowFrom(const hns_page_graph::GraphItem& graph_item) {
    return IsA<hns_page_graph::EdgeRequestStart>(
        DynamicTo<hns_page_graph::GraphEdge>(graph_item));
  }
};

}  // namespace blink

#endif  // HNS_THIRD_PARTY_BLINK_RENDERER_CORE_HNS_PAGE_GRAPH_GRAPH_ITEM_EDGE_REQUEST_EDGE_REQUEST_START_H_
