/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/third_party/blink/renderer/core/hns_page_graph/graph_item/edge/storage/edge_storage_clear.h"

#include "hns/third_party/blink/renderer/core/hns_page_graph/graph_item/node/actor/node_script.h"
#include "hns/third_party/blink/renderer/core/hns_page_graph/graph_item/node/storage/node_storage.h"

namespace hns_page_graph {

EdgeStorageClear::EdgeStorageClear(GraphItemContext* context,
                                   NodeScript* out_node,
                                   NodeStorage* in_node)
    : EdgeStorage(context, out_node, in_node, "") {}

EdgeStorageClear::~EdgeStorageClear() = default;

ItemName EdgeStorageClear::GetItemName() const {
  return "clear storage";
}

bool EdgeStorageClear::IsEdgeStorageClear() const {
  return true;
}

}  // namespace hns_page_graph
