/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_THIRD_PARTY_BLINK_RENDERER_CORE_HNS_PAGE_GRAPH_GRAPH_ITEM_EDGE_EVENT_LISTENER_EDGE_EVENT_LISTENER_ADD_H_
#define HNS_THIRD_PARTY_BLINK_RENDERER_CORE_HNS_PAGE_GRAPH_GRAPH_ITEM_EDGE_EVENT_LISTENER_EDGE_EVENT_LISTENER_ADD_H_

#include "hns/third_party/blink/renderer/core/hns_page_graph/graph_item/edge/event_listener/edge_event_listener_action.h"
#include "third_party/blink/renderer/platform/wtf/casting.h"
#include "third_party/blink/renderer/platform/wtf/text/wtf_string.h"

namespace hns_page_graph {

class NodeActor;
class NodeHTMLElement;

class EdgeEventListenerAdd final : public EdgeEventListenerAction {
 public:
  EdgeEventListenerAdd(GraphItemContext* context,
                       NodeActor* out_node,
                       NodeHTMLElement* in_node,
                       const String& event_type,
                       const EventListenerId listener_id,
                       NodeActor* listener_script);
  ~EdgeEventListenerAdd() override;

  ItemName GetItemName() const override;

  bool IsEdgeEventListenerAdd() const override;
};

}  // namespace hns_page_graph

namespace blink {

template <>
struct DowncastTraits<hns_page_graph::EdgeEventListenerAdd> {
  static bool AllowFrom(const hns_page_graph::EdgeEventListenerAction&
                            event_listener_action_edge) {
    return event_listener_action_edge.IsEdgeEventListenerAdd();
  }
  static bool AllowFrom(const hns_page_graph::GraphEdge& edge) {
    return IsA<hns_page_graph::EdgeEventListenerAdd>(
        DynamicTo<hns_page_graph::EdgeEventListenerAction>(edge));
  }
  static bool AllowFrom(const hns_page_graph::GraphItem& graph_item) {
    return IsA<hns_page_graph::EdgeEventListenerAdd>(
        DynamicTo<hns_page_graph::GraphEdge>(graph_item));
  }
};

}  // namespace blink

#endif  // HNS_THIRD_PARTY_BLINK_RENDERER_CORE_HNS_PAGE_GRAPH_GRAPH_ITEM_EDGE_EVENT_LISTENER_EDGE_EVENT_LISTENER_ADD_H_
