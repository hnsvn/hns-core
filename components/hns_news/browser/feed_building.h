// Copyright (c) 2021 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef HNS_COMPONENTS_HNS_NEWS_BROWSER_FEED_BUILDING_H_
#define HNS_COMPONENTS_HNS_NEWS_BROWSER_FEED_BUILDING_H_

#include <string>
#include <unordered_set>
#include <vector>

#include "base/containers/flat_map.h"
#include "hns/components/hns_news/browser/channels_controller.h"
#include "hns/components/hns_news/browser/publishers_parsing.h"
#include "hns/components/hns_news/common/hns_news.mojom-forward.h"
#include "hns/components/hns_news/common/hns_news.mojom.h"
#include "components/prefs/pref_service.h"

namespace hns_news {

bool BuildFeed(const std::vector<mojom::FeedItemPtr>& feed_items,
               const std::unordered_set<std::string>& history_hosts,
               Publishers* publishers,
               mojom::Feed* feed,
               PrefService* prefs);

// Exposed for testing
bool ShouldDisplayFeedItem(const mojom::FeedItemPtr& feed_item,
                           const Publishers* publishers,
                           const Channels& channels);

}  // namespace hns_news

#endif  // HNS_COMPONENTS_HNS_NEWS_BROWSER_FEED_BUILDING_H_
