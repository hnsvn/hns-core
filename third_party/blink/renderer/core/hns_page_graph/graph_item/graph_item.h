/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_THIRD_PARTY_BLINK_RENDERER_CORE_HNS_PAGE_GRAPH_GRAPH_ITEM_GRAPH_ITEM_H_
#define HNS_THIRD_PARTY_BLINK_RENDERER_CORE_HNS_PAGE_GRAPH_GRAPH_ITEM_GRAPH_ITEM_H_

#include <libxml/tree.h>

#include "base/memory/raw_ptr.h"
#include "base/time/time.h"
#include "hns/third_party/blink/renderer/core/hns_page_graph/types.h"

namespace hns_page_graph {

class GraphItemContext;

class GraphItem {
 public:
  explicit GraphItem(GraphItemContext* context);
  virtual ~GraphItem();

  GraphItemId GetId() const { return id_; }
  base::TimeTicks GetTimestamp() const { return time_; }

  virtual ItemName GetItemName() const = 0;
  virtual ItemDesc GetItemDesc() const;

  virtual GraphMLId GetGraphMLId() const = 0;
  virtual void AddGraphMLTag(xmlDocPtr doc, xmlNodePtr parent_node) const = 0;
  virtual void AddGraphMLAttributes(xmlDocPtr doc,
                                    xmlNodePtr parent_node) const;

  virtual bool IsEdge() const;
  virtual bool IsNode() const;

  base::TimeDelta GetTimeDeltaSincePageStart() const;

  GraphItemContext* GetContext() const { return context_; }

 private:
  GraphItemContext* const context_;
  const GraphItemId id_;
  const base::TimeTicks time_;
};

}  // namespace hns_page_graph

#endif  // HNS_THIRD_PARTY_BLINK_RENDERER_CORE_HNS_PAGE_GRAPH_GRAPH_ITEM_GRAPH_ITEM_H_
