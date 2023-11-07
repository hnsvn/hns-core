/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_THIRD_PARTY_BLINK_RENDERER_CORE_HNS_PAGE_GRAPH_GRAPH_ITEM_EDGE_JS_EDGE_JS_CALL_H_
#define HNS_THIRD_PARTY_BLINK_RENDERER_CORE_HNS_PAGE_GRAPH_GRAPH_ITEM_EDGE_JS_EDGE_JS_CALL_H_

#include "hns/third_party/blink/renderer/core/hns_page_graph/graph_item/edge/js/edge_js.h"
#include "third_party/blink/renderer/platform/wtf/casting.h"
#include "third_party/blink/renderer/platform/wtf/text/wtf_string.h"
#include "third_party/blink/renderer/platform/wtf/vector.h"

namespace hns_page_graph {

class NodeScript;
class NodeJS;

class EdgeJSCall final : public EdgeJS {
 public:
  EdgeJSCall(GraphItemContext* context,
             NodeScript* out_node,
             NodeJS* in_node,
             Vector<String> arguments,
             const int script_position);

  ~EdgeJSCall() override;

  const MethodName& GetMethodName() const override;

  const Vector<String>& GetArguments() const { return arguments_; }

  int GetScriptPosition() const { return script_position_; }

  ItemName GetItemName() const override;
  ItemDesc GetItemDesc() const override;

  void AddGraphMLAttributes(xmlDocPtr doc,
                            xmlNodePtr parent_node) const override;

  bool IsEdgeJSCall() const override;

 private:
  const Vector<String> arguments_;
  const int script_position_;
};

}  // namespace hns_page_graph

namespace blink {

template <>
struct DowncastTraits<hns_page_graph::EdgeJSCall> {
  static bool AllowFrom(const hns_page_graph::EdgeJS& edge) {
    return edge.IsEdgeJSCall();
  }
  static bool AllowFrom(const hns_page_graph::GraphEdge& edge) {
    return IsA<hns_page_graph::EdgeJSCall>(
        DynamicTo<hns_page_graph::EdgeJS>(edge));
  }
  static bool AllowFrom(const hns_page_graph::GraphItem& graph_item) {
    return IsA<hns_page_graph::EdgeJSCall>(
        DynamicTo<hns_page_graph::GraphEdge>(graph_item));
  }
};

}  // namespace blink

#endif  // HNS_THIRD_PARTY_BLINK_RENDERER_CORE_HNS_PAGE_GRAPH_GRAPH_ITEM_EDGE_JS_EDGE_JS_CALL_H_
