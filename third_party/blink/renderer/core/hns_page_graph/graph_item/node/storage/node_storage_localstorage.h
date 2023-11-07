/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_THIRD_PARTY_BLINK_RENDERER_CORE_HNS_PAGE_GRAPH_GRAPH_ITEM_NODE_STORAGE_NODE_STORAGE_LOCALSTORAGE_H_
#define HNS_THIRD_PARTY_BLINK_RENDERER_CORE_HNS_PAGE_GRAPH_GRAPH_ITEM_NODE_STORAGE_NODE_STORAGE_LOCALSTORAGE_H_

#include "hns/third_party/blink/renderer/core/hns_page_graph/graph_item/node/storage/node_storage.h"
#include "third_party/blink/renderer/platform/wtf/casting.h"

namespace hns_page_graph {

class NodeStorageLocalStorage final : public NodeStorage {
 public:
  explicit NodeStorageLocalStorage(GraphItemContext* context);
  ~NodeStorageLocalStorage() override;

  ItemName GetItemName() const override;

  bool IsNodeStorageLocalStorage() const override;
};

}  // namespace hns_page_graph

namespace blink {

template <>
struct DowncastTraits<hns_page_graph::NodeStorageLocalStorage> {
  static bool AllowFrom(const hns_page_graph::NodeStorage& node) {
    return node.IsNodeStorageLocalStorage();
  }
  static bool AllowFrom(const hns_page_graph::GraphNode& node) {
    return IsA<hns_page_graph::NodeStorageLocalStorage>(
        DynamicTo<hns_page_graph::NodeStorage>(node));
  }
  static bool AllowFrom(const hns_page_graph::GraphItem& graph_item) {
    return IsA<hns_page_graph::NodeStorageLocalStorage>(
        DynamicTo<hns_page_graph::GraphNode>(graph_item));
  }
};

}  // namespace blink

#endif  // HNS_THIRD_PARTY_BLINK_RENDERER_CORE_HNS_PAGE_GRAPH_GRAPH_ITEM_NODE_STORAGE_NODE_STORAGE_LOCALSTORAGE_H_
