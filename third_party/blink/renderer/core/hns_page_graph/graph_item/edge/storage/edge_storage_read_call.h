/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_THIRD_PARTY_BLINK_RENDERER_CORE_HNS_PAGE_GRAPH_GRAPH_ITEM_EDGE_STORAGE_EDGE_STORAGE_READ_CALL_H_
#define HNS_THIRD_PARTY_BLINK_RENDERER_CORE_HNS_PAGE_GRAPH_GRAPH_ITEM_EDGE_STORAGE_EDGE_STORAGE_READ_CALL_H_

#include "hns/third_party/blink/renderer/core/hns_page_graph/graph_item/edge/storage/edge_storage.h"
#include "third_party/blink/renderer/platform/wtf/casting.h"
#include "third_party/blink/renderer/platform/wtf/text/wtf_string.h"

namespace hns_page_graph {

class NodeScript;
class NodeStorage;

class EdgeStorageReadCall final : public EdgeStorage {
 public:
  EdgeStorageReadCall(GraphItemContext* context,
                      NodeScript* out_node,
                      NodeStorage* in_node,
                      const String& key);
  ~EdgeStorageReadCall() override;

  ItemName GetItemName() const override;

  bool IsEdgeStorageReadCall() const override;
};

}  // namespace hns_page_graph

namespace blink {

template <>
struct DowncastTraits<hns_page_graph::EdgeStorageReadCall> {
  static bool AllowFrom(const hns_page_graph::EdgeStorage& storage_edge) {
    return storage_edge.IsEdgeStorageReadCall();
  }
  static bool AllowFrom(const hns_page_graph::GraphEdge& edge) {
    return IsA<hns_page_graph::EdgeStorageReadCall>(
        DynamicTo<hns_page_graph::EdgeStorage>(edge));
  }
  static bool AllowFrom(const hns_page_graph::GraphItem& graph_item) {
    return IsA<hns_page_graph::EdgeStorageReadCall>(
        DynamicTo<hns_page_graph::GraphEdge>(graph_item));
  }
};

}  // namespace blink

#endif  // HNS_THIRD_PARTY_BLINK_RENDERER_CORE_HNS_PAGE_GRAPH_GRAPH_ITEM_EDGE_STORAGE_EDGE_STORAGE_READ_CALL_H_
