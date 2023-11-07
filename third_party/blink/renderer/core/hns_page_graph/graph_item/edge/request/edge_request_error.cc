/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/third_party/blink/renderer/core/hns_page_graph/graph_item/edge/request/edge_request_error.h"

#include "hns/third_party/blink/renderer/core/hns_page_graph/graph_item/node/node_resource.h"
#include "hns/third_party/blink/renderer/core/hns_page_graph/graphml.h"

namespace hns_page_graph {

EdgeRequestError::EdgeRequestError(GraphItemContext* context,
                                   NodeResource* out_node,
                                   GraphNode* in_node,
                                   const InspectorId request_id,
                                   const ResponseMetadata& metadata)
    : EdgeRequestResponse(context,
                          out_node,
                          in_node,
                          request_id,
                          kRequestStatusError,
                          metadata) {}

EdgeRequestError::~EdgeRequestError() = default;

ItemName EdgeRequestError::GetItemName() const {
  return "request error";
}

bool EdgeRequestError::IsEdgeRequestError() const {
  return true;
}

}  // namespace hns_page_graph
