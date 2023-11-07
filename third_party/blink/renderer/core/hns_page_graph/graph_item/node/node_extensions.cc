/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/third_party/blink/renderer/core/hns_page_graph/graph_item/node/node_extensions.h"

namespace hns_page_graph {

NodeExtensions::NodeExtensions(GraphItemContext* context)
    : GraphNode(context) {}

NodeExtensions::~NodeExtensions() = default;

ItemName NodeExtensions::GetItemName() const {
  return "extensions";
}

bool NodeExtensions::IsNodeExtensions() const {
  return true;
}

}  // namespace hns_page_graph
