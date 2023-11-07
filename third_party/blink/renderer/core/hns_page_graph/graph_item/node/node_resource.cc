/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/third_party/blink/renderer/core/hns_page_graph/graph_item/node/node_resource.h"

#include "hns/third_party/blink/renderer/core/hns_page_graph/graph_item/edge/edge_resource_block.h"
#include "hns/third_party/blink/renderer/core/hns_page_graph/graph_item/edge/request/edge_request_response.h"
#include "hns/third_party/blink/renderer/core/hns_page_graph/graph_item/edge/request/edge_request_start.h"
#include "hns/third_party/blink/renderer/core/hns_page_graph/graphml.h"
#include "third_party/blink/renderer/platform/wtf/text/text_stream.h"

namespace hns_page_graph {

NodeResource::NodeResource(GraphItemContext* context, const RequestURL url)
    : GraphNode(context), url_(url) {}

NodeResource::~NodeResource() = default;

ItemName NodeResource::GetItemName() const {
  return "resource";
}

ItemDesc NodeResource::GetItemDesc() const {
  WTF::TextStream ts;
  ts << GraphNode::GetItemDesc() << " [" << url_.GetString() << "]";
  return ts.Release();
}

void NodeResource::AddGraphMLAttributes(xmlDocPtr doc,
                                        xmlNodePtr parent_node) const {
  GraphNode::AddGraphMLAttributes(doc, parent_node);
  GraphMLAttrDefForType(kGraphMLAttrDefURL)
      ->AddValueNode(doc, parent_node, url_.GetString());
}

bool NodeResource::IsNodeResource() const {
  return true;
}

}  // namespace hns_page_graph
