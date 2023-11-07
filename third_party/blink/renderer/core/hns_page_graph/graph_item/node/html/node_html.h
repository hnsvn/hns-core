/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_THIRD_PARTY_BLINK_RENDERER_CORE_HNS_PAGE_GRAPH_GRAPH_ITEM_NODE_HTML_NODE_HTML_H_
#define HNS_THIRD_PARTY_BLINK_RENDERER_CORE_HNS_PAGE_GRAPH_GRAPH_ITEM_NODE_HTML_NODE_HTML_H_

#include "hns/third_party/blink/renderer/core/hns_page_graph/graph_item/node/graph_node.h"
#include "third_party/blink/renderer/platform/wtf/casting.h"

namespace hns_page_graph {

class NodeHTMLElement;

class NodeHTML : public GraphNode {
 public:
  NodeHTML(GraphItemContext* context, const blink::DOMNodeId dom_node_id);
  ~NodeHTML() override;

  blink::DOMNodeId GetDOMNodeId() const { return dom_node_id_; }

  NodeHTMLElement* GetParentNode() const { return parent_node_; }

  bool IsDeleted() const { return is_deleted_; }

  ItemDesc GetItemDesc() const override;

  void AddGraphMLAttributes(xmlDocPtr doc,
                            xmlNodePtr parent_node) const override;

  bool IsNodeHTML() const override;

  virtual bool IsNodeHTMLElement() const;
  virtual bool IsNodeHTMLText() const;

  void SetParentNode(NodeHTMLElement* parent_node) {
    parent_node_ = parent_node;
  }

  virtual void MarkDeleted();

 protected:
  void AddInEdge(const GraphEdge* in_edge) override;

 private:
  const blink::DOMNodeId dom_node_id_;
  NodeHTMLElement* parent_node_;
  bool is_deleted_;
};

}  // namespace hns_page_graph

namespace blink {

template <>
struct DowncastTraits<hns_page_graph::NodeHTML> {
  static bool AllowFrom(const hns_page_graph::GraphNode& node) {
    return node.IsNodeHTML();
  }
  static bool AllowFrom(const hns_page_graph::GraphItem& graph_item) {
    return IsA<hns_page_graph::NodeHTML>(
        DynamicTo<hns_page_graph::GraphNode>(graph_item));
  }
};

}  // namespace blink

#endif  // HNS_THIRD_PARTY_BLINK_RENDERER_CORE_HNS_PAGE_GRAPH_GRAPH_ITEM_NODE_HTML_NODE_HTML_H_
