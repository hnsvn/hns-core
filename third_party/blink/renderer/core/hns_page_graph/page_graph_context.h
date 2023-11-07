/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_THIRD_PARTY_BLINK_RENDERER_CORE_HNS_PAGE_GRAPH_PAGE_GRAPH_CONTEXT_H_
#define HNS_THIRD_PARTY_BLINK_RENDERER_CORE_HNS_PAGE_GRAPH_PAGE_GRAPH_CONTEXT_H_

#include <memory>
#include <type_traits>
#include <utility>

#include "hns/third_party/blink/renderer/core/hns_page_graph/graph_item/graph_item_context.h"

namespace hns_page_graph {

class GraphNode;
class GraphEdge;

class PageGraphContext : public GraphItemContext {
 public:
  virtual void AddGraphItem(std::unique_ptr<GraphItem> graph_item) = 0;

  template <typename T, typename... Args>
  T* AddNode(Args&&... args) {
    static_assert(std::is_base_of<GraphNode, T>::value,
                  "AddNode only for Nodes");
    T* node = new T(this, std::forward<Args>(args)...);
    AddGraphItem(std::unique_ptr<T>(node));
    return node;
  }

  template <typename T, typename... Args>
  T* AddEdge(Args&&... args) {
    static_assert(std::is_base_of<GraphEdge, T>::value,
                  "AddEdge only for Edges");
    T* edge = new T(this, std::forward<Args>(args)...);
    AddGraphItem(std::unique_ptr<T>(edge));
    return edge;
  }
};

}  // namespace hns_page_graph

#endif  // HNS_THIRD_PARTY_BLINK_RENDERER_CORE_HNS_PAGE_GRAPH_PAGE_GRAPH_CONTEXT_H_
