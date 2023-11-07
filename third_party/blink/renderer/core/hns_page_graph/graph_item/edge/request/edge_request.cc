/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/third_party/blink/renderer/core/hns_page_graph/graph_item/edge/request/edge_request.h"

#include "hns/third_party/blink/renderer/core/hns_page_graph/graph_item/node/node_resource.h"
#include "hns/third_party/blink/renderer/core/hns_page_graph/graphml.h"

namespace hns_page_graph {

EdgeRequest::EdgeRequest(GraphItemContext* context,
                         GraphNode* out_node,
                         GraphNode* in_node,
                         const InspectorId request_id,
                         const RequestStatus request_status)
    : GraphEdge(context, out_node, in_node),
      request_id_(request_id),
      request_status_(request_status) {}

EdgeRequest::~EdgeRequest() = default;

RequestURL EdgeRequest::GetRequestURL() const {
  return GetResourceNode()->GetURL();
}

void EdgeRequest::AddGraphMLAttributes(xmlDocPtr doc,
                                       xmlNodePtr parent_node) const {
  GraphEdge::AddGraphMLAttributes(doc, parent_node);
  GraphMLAttrDefForType(kGraphMLAttrDefRequestId)
      ->AddValueNode(doc, parent_node, request_id_);
  GraphMLAttrDefForType(kGraphMLAttrDefStatus)
      ->AddValueNode(doc, parent_node, RequestStatusToString(request_status_));
}

bool EdgeRequest::IsEdgeRequest() const {
  return true;
}

bool EdgeRequest::IsEdgeRequestStart() const {
  return false;
}

bool EdgeRequest::IsEdgeRequestResponse() const {
  return false;
}

}  // namespace hns_page_graph
