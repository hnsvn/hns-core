/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_THIRD_PARTY_BLINK_RENDERER_CORE_HNS_PAGE_GRAPH_GRAPH_ITEM_NODE_GRAPH_NODE_H_
#define HNS_THIRD_PARTY_BLINK_RENDERER_CORE_HNS_PAGE_GRAPH_GRAPH_ITEM_NODE_GRAPH_NODE_H_

#include "hns/third_party/blink/renderer/core/hns_page_graph/graph_item/graph_item.h"
#include "third_party/blink/renderer/platform/wtf/casting.h"

namespace hns_page_graph {

class GraphEdge;

class GraphNode : public GraphItem {
 public:
  explicit GraphNode(GraphItemContext* context);

  ~GraphNode() override;

  const EdgeList& GetInEdges() const { return in_edges_; }
  const EdgeList& GetOutEdges() const { return out_edges_; }

  virtual void AddInEdge(const GraphEdge* in_edge);
  virtual void AddOutEdge(const GraphEdge* out_edge);

  GraphMLId GetGraphMLId() const override;
  void AddGraphMLTag(xmlDocPtr doc, xmlNodePtr parent_node) const override;
  void AddGraphMLAttributes(xmlDocPtr doc,
                            xmlNodePtr parent_node) const override;

  bool IsNode() const override;

  virtual bool IsNodeActor() const;
  virtual bool IsNodeBinding() const;
  virtual bool IsNodeBindingEvent() const;
  virtual bool IsNodeExtensions() const;
  virtual bool IsNodeFilter() const;
  virtual bool IsNodeHTML() const;
  virtual bool IsNodeJS() const;
  virtual bool IsNodeRemoteFrame() const;
  virtual bool IsNodeResource() const;
  virtual bool IsNodeShield() const;
  virtual bool IsNodeShields() const;
  virtual bool IsNodeStorage() const;
  virtual bool IsNodeStorageRoot() const;

 private:
  // Reminder to self:
  //   out_edge -> node -> in_edge
  // These vectors do not own their references.  All nodes in the entire
  // context are owned by the GraphItemContext instance.
  EdgeList in_edges_;
  EdgeList out_edges_;
};

}  // namespace hns_page_graph

namespace blink {

template <>
struct DowncastTraits<hns_page_graph::GraphNode> {
  static bool AllowFrom(const hns_page_graph::GraphItem& graph_item) {
    return graph_item.IsNode();
  }
};

}  // namespace blink

#endif  // HNS_THIRD_PARTY_BLINK_RENDERER_CORE_HNS_PAGE_GRAPH_GRAPH_ITEM_NODE_GRAPH_NODE_H_
