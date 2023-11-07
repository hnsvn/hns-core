/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_THIRD_PARTY_BLINK_RENDERER_CORE_HNS_PAGE_GRAPH_GRAPH_ITEM_EDGE_EVENT_LISTENER_EDGE_EVENT_LISTENER_H_
#define HNS_THIRD_PARTY_BLINK_RENDERER_CORE_HNS_PAGE_GRAPH_GRAPH_ITEM_EDGE_EVENT_LISTENER_EDGE_EVENT_LISTENER_H_

#include "hns/third_party/blink/renderer/core/hns_page_graph/graph_item/edge/graph_edge.h"
#include "third_party/blink/renderer/platform/wtf/casting.h"
#include "third_party/blink/renderer/platform/wtf/text/wtf_string.h"

namespace hns_page_graph {

class NodeActor;
class NodeHTMLElement;

class EdgeEventListener : public GraphEdge {
 public:
  EdgeEventListener(GraphItemContext* context,
                    NodeHTMLElement* out_node,
                    NodeActor* in_node,
                    const String& event_type,
                    const EventListenerId listener_id);
  ~EdgeEventListener() override;

  const String& GetEventType() const { return event_type_; }
  EventListenerId GetListenerId() const { return listener_id_; }

  ItemName GetItemName() const override;
  ItemDesc GetItemDesc() const override;

  void AddGraphMLAttributes(xmlDocPtr doc,
                            xmlNodePtr parent_node) const override;

  bool IsEdgeEventListener() const override;

 private:
  const String event_type_;
  const EventListenerId listener_id_;
};

}  // namespace hns_page_graph

namespace blink {

template <>
struct DowncastTraits<hns_page_graph::EdgeEventListener> {
  static bool AllowFrom(const hns_page_graph::GraphEdge& edge) {
    return edge.IsEdgeEventListener();
  }
  static bool AllowFrom(const hns_page_graph::GraphItem& graph_item) {
    return IsA<hns_page_graph::EdgeEventListener>(
        DynamicTo<hns_page_graph::GraphEdge>(graph_item));
  }
};

}  // namespace blink

#endif  // HNS_THIRD_PARTY_BLINK_RENDERER_CORE_HNS_PAGE_GRAPH_GRAPH_ITEM_EDGE_EVENT_LISTENER_EDGE_EVENT_LISTENER_H_
