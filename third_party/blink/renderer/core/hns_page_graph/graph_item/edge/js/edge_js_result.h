/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_THIRD_PARTY_BLINK_RENDERER_CORE_HNS_PAGE_GRAPH_GRAPH_ITEM_EDGE_JS_EDGE_JS_RESULT_H_
#define HNS_THIRD_PARTY_BLINK_RENDERER_CORE_HNS_PAGE_GRAPH_GRAPH_ITEM_EDGE_JS_EDGE_JS_RESULT_H_

#include "hns/third_party/blink/renderer/core/hns_page_graph/graph_item/edge/js/edge_js.h"
#include "third_party/blink/renderer/platform/wtf/casting.h"
#include "third_party/blink/renderer/platform/wtf/text/wtf_string.h"

namespace hns_page_graph {

class NodeScript;
class NodeJS;

class EdgeJSResult final : public EdgeJS {
 public:
  EdgeJSResult(GraphItemContext* context,
               NodeJS* out_node,
               NodeScript* in_node,
               const String& result);

  ~EdgeJSResult() override;

  ItemName GetItemName() const override;
  ItemDesc GetItemDesc() const override;

  void AddGraphMLAttributes(xmlDocPtr doc,
                            xmlNodePtr parent_node) const override;

  const String& GetResult() const;
  const MethodName& GetMethodName() const override;
  bool IsEdgeJSResult() const override;

 private:
  const String result_;
};

}  // namespace hns_page_graph

namespace blink {

template <>
struct DowncastTraits<hns_page_graph::EdgeJSResult> {
  static bool AllowFrom(const hns_page_graph::EdgeJS& edge) {
    return edge.IsEdgeJSResult();
  }
  static bool AllowFrom(const hns_page_graph::GraphEdge& edge) {
    return IsA<hns_page_graph::EdgeJSResult>(
        DynamicTo<hns_page_graph::EdgeJS>(edge));
  }
  static bool AllowFrom(const hns_page_graph::GraphItem& graph_item) {
    return IsA<hns_page_graph::EdgeJSResult>(
        DynamicTo<hns_page_graph::GraphEdge>(graph_item));
  }
};

}  // namespace blink

#endif  // HNS_THIRD_PARTY_BLINK_RENDERER_CORE_HNS_PAGE_GRAPH_GRAPH_ITEM_EDGE_JS_EDGE_JS_RESULT_H_
