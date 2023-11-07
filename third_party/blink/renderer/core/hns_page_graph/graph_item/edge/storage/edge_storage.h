/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_THIRD_PARTY_BLINK_RENDERER_CORE_HNS_PAGE_GRAPH_GRAPH_ITEM_EDGE_STORAGE_EDGE_STORAGE_H_
#define HNS_THIRD_PARTY_BLINK_RENDERER_CORE_HNS_PAGE_GRAPH_GRAPH_ITEM_EDGE_STORAGE_EDGE_STORAGE_H_

#include "hns/third_party/blink/renderer/core/hns_page_graph/graph_item/edge/graph_edge.h"
#include "third_party/blink/renderer/platform/wtf/casting.h"
#include "third_party/blink/renderer/platform/wtf/text/wtf_string.h"

namespace hns_page_graph {

class GraphNode;

class EdgeStorage : public GraphEdge {
 public:
  EdgeStorage(GraphItemContext* context,
              GraphNode* out_node,
              GraphNode* in_node,
              const String& key);
  ~EdgeStorage() override;

  const String& GetKey() const { return key_; }

  ItemName GetItemDesc() const override;

  void AddGraphMLAttributes(xmlDocPtr doc,
                            xmlNodePtr parent_node) const override;

  bool IsEdgeStorage() const override;

  virtual bool IsEdgeStorageClear() const;
  virtual bool IsEdgeStorageDelete() const;
  virtual bool IsEdgeStorageReadCall() const;
  virtual bool IsEdgeStorageReadResult() const;
  virtual bool IsEdgeStorageSet() const;

 private:
  const String key_;
};

}  // namespace hns_page_graph

namespace blink {

template <>
struct DowncastTraits<hns_page_graph::EdgeStorage> {
  static bool AllowFrom(const hns_page_graph::GraphEdge& edge) {
    return edge.IsEdgeStorage();
  }
  static bool AllowFrom(const hns_page_graph::GraphItem& graph_item) {
    return IsA<hns_page_graph::EdgeStorage>(
        DynamicTo<hns_page_graph::GraphEdge>(graph_item));
  }
};

}  // namespace blink

#endif  // HNS_THIRD_PARTY_BLINK_RENDERER_CORE_HNS_PAGE_GRAPH_GRAPH_ITEM_EDGE_STORAGE_EDGE_STORAGE_H_
