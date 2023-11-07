/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/third_party/blink/renderer/core/hns_page_graph/graph_item/edge/node/edge_node_delete.h"

#include "hns/third_party/blink/renderer/core/hns_page_graph/graph_item/node/actor/node_script.h"
#include "hns/third_party/blink/renderer/core/hns_page_graph/graph_item/node/html/node_html.h"

namespace hns_page_graph {

EdgeNodeDelete::EdgeNodeDelete(GraphItemContext* context,
                               NodeScript* out_node,
                               NodeHTML* in_node)
    : EdgeNode(context, out_node, in_node) {}

EdgeNodeDelete::~EdgeNodeDelete() = default;

ItemName EdgeNodeDelete::GetItemName() const {
  return "delete node";
}

bool EdgeNodeDelete::IsEdgeNodeDelete() const {
  return true;
}

}  // namespace hns_page_graph
