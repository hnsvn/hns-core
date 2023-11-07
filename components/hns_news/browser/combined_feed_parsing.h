// Copyright (c) 2021 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef HNS_COMPONENTS_HNS_NEWS_BROWSER_COMBINED_FEED_PARSING_H_
#define HNS_COMPONENTS_HNS_NEWS_BROWSER_COMBINED_FEED_PARSING_H_

#include <string>
#include <vector>

#include "base/values.h"
#include "hns/components/hns_news/common/hns_news.mojom.h"

namespace hns_news {

// Convert from the "combined feed" hosted remotely to Hns News mojom items.
std::vector<mojom::FeedItemPtr> ParseFeedItems(const base::Value& value);

}  // namespace hns_news

#endif  // HNS_COMPONENTS_HNS_NEWS_BROWSER_COMBINED_FEED_PARSING_H_
