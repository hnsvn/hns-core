/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/third_party/blink/renderer/core/hns_page_graph/graph_item/node/filter/node_ad_filter.h"

#include "hns/third_party/blink/renderer/core/hns_page_graph/graphml.h"
#include "third_party/blink/renderer/platform/wtf/text/text_stream.h"

namespace hns_page_graph {

NodeAdFilter::NodeAdFilter(GraphItemContext* context, const String& rule)
    : NodeFilter(context), rule_(rule) {}

NodeAdFilter::~NodeAdFilter() = default;

ItemName NodeAdFilter::GetItemName() const {
  return "ad filter";
}

ItemDesc NodeAdFilter::GetItemDesc() const {
  WTF::TextStream ts;
  ts << NodeFilter::GetItemDesc();
  if (!rule_.empty()) {
    ts << " [" << rule_ << "]";
  }
  return ts.Release();
}

void NodeAdFilter::AddGraphMLAttributes(xmlDocPtr doc,
                                        xmlNodePtr parent_node) const {
  NodeFilter::AddGraphMLAttributes(doc, parent_node);
  GraphMLAttrDefForType(kGraphMLAttrDefRule)
      ->AddValueNode(doc, parent_node, rule_);
}

bool NodeAdFilter::IsNodeAdFilter() const {
  return true;
}

}  // namespace hns_page_graph
