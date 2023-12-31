/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/third_party/blink/renderer/core/hns_page_graph/graph_item/node/js/node_js.h"

namespace hns_page_graph {

NodeJS::NodeJS(GraphItemContext* context) : GraphNode(context) {}

NodeJS::~NodeJS() = default;

bool NodeJS::IsNodeJS() const {
  return true;
}

bool NodeJS::IsNodeJSBuiltin() const {
  return false;
}

bool NodeJS::IsNodeJSWebAPI() const {
  return false;
}

}  // namespace hns_page_graph
