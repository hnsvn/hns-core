/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/third_party/blink/renderer/core/hns_page_graph/graph_item/edge/binding/edge_binding_event.h"

#include "hns/third_party/blink/renderer/core/hns_page_graph/graph_item/node/actor/node_script.h"
#include "hns/third_party/blink/renderer/core/hns_page_graph/graph_item/node/binding/node_binding_event.h"
#include "hns/third_party/blink/renderer/core/hns_page_graph/graphml.h"

namespace hns_page_graph {

EdgeBindingEvent::EdgeBindingEvent(GraphItemContext* context,
                                   NodeScript* out_node,
                                   NodeBindingEvent* in_node,
                                   const int script_position)
    : GraphEdge(context, out_node, in_node),
      script_position_(script_position) {}

EdgeBindingEvent::~EdgeBindingEvent() = default;

ItemName EdgeBindingEvent::GetItemName() const {
  return "binding event";
}

ItemDesc EdgeBindingEvent::GetItemDesc() const {
  return GetItemName();
}

void EdgeBindingEvent::AddGraphMLAttributes(xmlDocPtr doc,
                                            xmlNodePtr parent_node) const {
  GraphEdge::AddGraphMLAttributes(doc, parent_node);
  GraphMLAttrDefForType(kGraphMLAttrDefScriptPosition)
      ->AddValueNode(doc, parent_node, script_position_);
}

bool EdgeBindingEvent::IsEdgeBindingEvent() const {
  return true;
}

}  // namespace hns_page_graph
