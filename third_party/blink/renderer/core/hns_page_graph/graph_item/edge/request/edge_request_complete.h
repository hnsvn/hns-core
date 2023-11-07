/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_THIRD_PARTY_BLINK_RENDERER_CORE_HNS_PAGE_GRAPH_GRAPH_ITEM_EDGE_REQUEST_EDGE_REQUEST_COMPLETE_H_
#define HNS_THIRD_PARTY_BLINK_RENDERER_CORE_HNS_PAGE_GRAPH_GRAPH_ITEM_EDGE_REQUEST_EDGE_REQUEST_COMPLETE_H_

#include "hns/third_party/blink/renderer/core/hns_page_graph/graph_item/edge/request/edge_request_response.h"
#include "hns/third_party/blink/renderer/core/hns_page_graph/utilities/response_metadata.h"
#include "third_party/blink/renderer/platform/wtf/casting.h"
#include "third_party/blink/renderer/platform/wtf/text/wtf_string.h"

namespace hns_page_graph {

class GraphNode;
class NodeResource;

class EdgeRequestComplete final : public EdgeRequestResponse {
 public:
  EdgeRequestComplete(GraphItemContext* context,
                      NodeResource* out_node,
                      GraphNode* in_node,
                      const InspectorId request_id,
                      const String& resource_type,
                      const ResponseMetadata& metadata,
                      const String& hash);

  ~EdgeRequestComplete() override;

  const String& GetResourceType() const { return resource_type_; }

  ItemName GetItemName() const override;
  ItemDesc GetItemDesc() const override;

  void AddGraphMLAttributes(xmlDocPtr doc,
                            xmlNodePtr parent_node) const override;

  bool IsEdgeRequestComplete() const override;

 private:
  const String resource_type_;
  const String hash_;
};

}  // namespace hns_page_graph

namespace blink {

template <>
struct DowncastTraits<hns_page_graph::EdgeRequestComplete> {
  static bool AllowFrom(
      const hns_page_graph::EdgeRequestResponse& request_response_edge) {
    return request_response_edge.IsEdgeRequestComplete();
  }
  static bool AllowFrom(const hns_page_graph::EdgeRequest& request_edge) {
    return IsA<hns_page_graph::EdgeRequestComplete>(
        DynamicTo<hns_page_graph::EdgeRequestResponse>(request_edge));
  }
  static bool AllowFrom(const hns_page_graph::GraphEdge& edge) {
    return IsA<hns_page_graph::EdgeRequestComplete>(
        DynamicTo<hns_page_graph::EdgeRequest>(edge));
  }
  static bool AllowFrom(const hns_page_graph::GraphItem& graph_item) {
    return IsA<hns_page_graph::EdgeRequestComplete>(
        DynamicTo<hns_page_graph::GraphEdge>(graph_item));
  }
};

}  // namespace blink

#endif  // HNS_THIRD_PARTY_BLINK_RENDERER_CORE_HNS_PAGE_GRAPH_GRAPH_ITEM_EDGE_REQUEST_EDGE_REQUEST_COMPLETE_H_
