/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_THIRD_PARTY_BLINK_RENDERER_CORE_HNS_PAGE_GRAPH_GRAPH_ITEM_NODE_JS_NODE_JS_BUILTIN_H_
#define HNS_THIRD_PARTY_BLINK_RENDERER_CORE_HNS_PAGE_GRAPH_GRAPH_ITEM_NODE_JS_NODE_JS_BUILTIN_H_

#include "hns/third_party/blink/renderer/core/hns_page_graph/graph_item/node/js/node_js.h"
#include "third_party/blink/renderer/platform/wtf/casting.h"

namespace hns_page_graph {

class NodeJSBuiltin final : public NodeJS {
 public:
  NodeJSBuiltin(GraphItemContext* context, const MethodName& builtin);
  ~NodeJSBuiltin() override;

  const MethodName& GetMethodName() const override;

  ItemName GetItemName() const override;
  ItemDesc GetItemDesc() const override;

  void AddGraphMLAttributes(xmlDocPtr doc,
                            xmlNodePtr parent_node) const override;

  bool IsNodeJSBuiltin() const override;

 private:
  const MethodName builtin_;
};

}  // namespace hns_page_graph

namespace blink {

template <>
struct DowncastTraits<hns_page_graph::NodeJSBuiltin> {
  static bool AllowFrom(const hns_page_graph::NodeJS& js_node) {
    return js_node.IsNodeJSBuiltin();
  }
  static bool AllowFrom(const hns_page_graph::GraphNode& node) {
    return IsA<hns_page_graph::NodeJSBuiltin>(
        DynamicTo<hns_page_graph::NodeJS>(node));
  }
  static bool AllowFrom(const hns_page_graph::GraphItem& graph_item) {
    return IsA<hns_page_graph::NodeJSBuiltin>(
        DynamicTo<hns_page_graph::GraphNode>(graph_item));
  }
};

}  // namespace blink

#endif  // HNS_THIRD_PARTY_BLINK_RENDERER_CORE_HNS_PAGE_GRAPH_GRAPH_ITEM_NODE_JS_NODE_JS_BUILTIN_H_
