/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/third_party/blink/renderer/core/hns_page_graph/graph_item/node/storage/node_storage_sessionstorage.h"

namespace hns_page_graph {

NodeStorageSessionStorage::NodeStorageSessionStorage(GraphItemContext* context)
    : NodeStorage(context) {}

NodeStorageSessionStorage::~NodeStorageSessionStorage() = default;

ItemName NodeStorageSessionStorage::GetItemName() const {
  return "session storage";
}

bool NodeStorageSessionStorage::IsNodeStorageSessionStorage() const {
  return true;
}

}  // namespace hns_page_graph
