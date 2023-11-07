/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/third_party/blink/renderer/core/hns_page_graph/graph_item/node/shield/node_shield.h"

namespace hns_page_graph {

NodeShield::NodeShield(GraphItemContext* context, const String& type)
    : GraphNode(context), type_(type) {}

NodeShield::~NodeShield() = default;

ItemName NodeShield::GetItemName() const {
  return type_ + " shield";
}

bool NodeShield::IsNodeShield() const {
  return true;
}

}  // namespace hns_page_graph
