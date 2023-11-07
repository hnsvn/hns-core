/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/third_party/blink/renderer/core/hns_page_graph/graph_item/edge/request/edge_request_response.h"

#include "base/strings/string_number_conversions.h"
#include "hns/third_party/blink/renderer/core/hns_page_graph/graph_item/node/node_resource.h"
#include "hns/third_party/blink/renderer/core/hns_page_graph/graphml.h"

namespace hns_page_graph {

EdgeRequestResponse::EdgeRequestResponse(GraphItemContext* context,
                                         NodeResource* out_node,
                                         GraphNode* in_node,
                                         const InspectorId request_id,
                                         const RequestStatus request_status,
                                         const ResponseMetadata& metadata)
    : EdgeRequest(context, out_node, in_node, request_id, request_status),
      response_header_string_(metadata.GetResponseHeaderSummary()),
      response_data_length_(metadata.EncodedDataLength()) {}

EdgeRequestResponse::~EdgeRequestResponse() = default;

NodeResource* EdgeRequestResponse::GetResourceNode() const {
  return static_cast<NodeResource*>(GetOutNode());
}

GraphNode* EdgeRequestResponse::GetRequestingNode() const {
  return GetInNode();
}

ItemName EdgeRequestResponse::GetItemName() const {
  return "request response";
}

void EdgeRequestResponse::AddGraphMLAttributes(xmlDocPtr doc,
                                               xmlNodePtr parent_node) const {
  EdgeRequest::AddGraphMLAttributes(doc, parent_node);
  GraphMLAttrDefForType(kGraphMLAttrDefHeaders)
      ->AddValueNode(doc, parent_node, response_header_string_);
  GraphMLAttrDefForType(kGraphMLAttrDefSize)
      ->AddValueNode(doc, parent_node,
                     base::NumberToString(response_data_length_));
}

bool EdgeRequestResponse::IsEdgeRequestResponse() const {
  return true;
}

bool EdgeRequestResponse::IsEdgeRequestComplete() const {
  return false;
}

bool EdgeRequestResponse::IsEdgeRequestError() const {
  return false;
}

}  // namespace hns_page_graph
