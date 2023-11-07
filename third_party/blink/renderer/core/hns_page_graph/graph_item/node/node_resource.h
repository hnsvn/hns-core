/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_THIRD_PARTY_BLINK_RENDERER_CORE_HNS_PAGE_GRAPH_GRAPH_ITEM_NODE_NODE_RESOURCE_H_
#define HNS_THIRD_PARTY_BLINK_RENDERER_CORE_HNS_PAGE_GRAPH_GRAPH_ITEM_NODE_NODE_RESOURCE_H_

#include "hns/third_party/blink/renderer/core/hns_page_graph/graph_item/node/graph_node.h"
#include "third_party/blink/renderer/platform/wtf/casting.h"

namespace hns_page_graph {

class NodeResource : public GraphNode {
 public:
  NodeResource(GraphItemContext* context, const RequestURL url);
  ~NodeResource() override;

  RequestURL GetURL() const { return url_; }

  ItemName GetItemName() const override;
  ItemDesc GetItemDesc() const override;

  void AddGraphMLAttributes(xmlDocPtr doc,
                            xmlNodePtr parent_node) const override;

  bool IsNodeResource() const override;

 private:
  const RequestURL url_;
};

}  // namespace hns_page_graph

namespace blink {

template <>
struct DowncastTraits<hns_page_graph::NodeResource> {
  static bool AllowFrom(const hns_page_graph::GraphNode& node) {
    return node.IsNodeResource();
  }
  static bool AllowFrom(const hns_page_graph::GraphItem& graph_item) {
    return IsA<hns_page_graph::NodeResource>(
        DynamicTo<hns_page_graph::GraphNode>(graph_item));
  }
};

}  // namespace blink

#endif  // HNS_THIRD_PARTY_BLINK_RENDERER_CORE_HNS_PAGE_GRAPH_GRAPH_ITEM_NODE_NODE_RESOURCE_H_
