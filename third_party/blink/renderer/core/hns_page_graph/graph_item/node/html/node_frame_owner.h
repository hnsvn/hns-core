/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_THIRD_PARTY_BLINK_RENDERER_CORE_HNS_PAGE_GRAPH_GRAPH_ITEM_NODE_HTML_NODE_FRAME_OWNER_H_
#define HNS_THIRD_PARTY_BLINK_RENDERER_CORE_HNS_PAGE_GRAPH_GRAPH_ITEM_NODE_HTML_NODE_FRAME_OWNER_H_

#include "hns/third_party/blink/renderer/core/hns_page_graph/graph_item/node/html/node_html_element.h"
#include "third_party/blink/renderer/platform/wtf/casting.h"
#include "third_party/blink/renderer/platform/wtf/text/wtf_string.h"

namespace hns_page_graph {

class NodeFrameOwner final : public NodeHTMLElement {
 public:
  NodeFrameOwner(GraphItemContext* context,
                 const blink::DOMNodeId dom_node_id,
                 const String& tag_name);

  ItemName GetItemName() const override;

  bool IsNodeFrameOwner() const override;
};

}  // namespace hns_page_graph

namespace blink {

template <>
struct DowncastTraits<hns_page_graph::NodeFrameOwner> {
  static bool AllowFrom(const hns_page_graph::NodeHTMLElement& element_node) {
    return element_node.IsNodeFrameOwner();
  }
  static bool AllowFrom(const hns_page_graph::NodeHTML& html_node) {
    return IsA<hns_page_graph::NodeFrameOwner>(
        DynamicTo<hns_page_graph::NodeHTMLElement>(html_node));
  }
  static bool AllowFrom(const hns_page_graph::GraphNode& node) {
    return IsA<hns_page_graph::NodeFrameOwner>(
        DynamicTo<hns_page_graph::NodeHTML>(node));
  }
  static bool AllowFrom(const hns_page_graph::GraphItem& graph_item) {
    return IsA<hns_page_graph::NodeFrameOwner>(
        DynamicTo<hns_page_graph::GraphNode>(graph_item));
  }
};

}  // namespace blink

#endif  // HNS_THIRD_PARTY_BLINK_RENDERER_CORE_HNS_PAGE_GRAPH_GRAPH_ITEM_NODE_HTML_NODE_FRAME_OWNER_H_
