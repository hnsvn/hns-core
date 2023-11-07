/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/third_party/blink/renderer/core/hns_page_graph/graph_item/graph_item.h"

#include "hns/third_party/blink/renderer/core/hns_page_graph/graph_item/graph_item_context.h"
#include "hns/third_party/blink/renderer/core/hns_page_graph/graphml.h"
#include "third_party/blink/renderer/platform/wtf/text/text_stream.h"

namespace hns_page_graph {

GraphItem::GraphItem(GraphItemContext* context)
    : context_(context),
      id_(context->GetNextGraphItemId()),
      time_(base::TimeTicks::Now()) {}

GraphItem::~GraphItem() = default;

ItemDesc GraphItem::GetItemDesc() const {
  WTF::TextStream ts;
  ts << GetItemName() << " #" << id_;
  return ts.Release();
}

void GraphItem::AddGraphMLAttributes(xmlDocPtr doc,
                                     xmlNodePtr parent_node) const {}

bool GraphItem::IsEdge() const {
  return false;
}

bool GraphItem::IsNode() const {
  return false;
}

base::TimeDelta GraphItem::GetTimeDeltaSincePageStart() const {
  return time_ - context_->GetGraphStartTime();
}

}  // namespace hns_page_graph
