// Copyright (c) 2021 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef HNS_COMPONENTS_HNS_NEWS_BROWSER_PUBLISHERS_PARSING_H_
#define HNS_COMPONENTS_HNS_NEWS_BROWSER_PUBLISHERS_PARSING_H_

#include <string>
#include <vector>

#include "base/values.h"
#include "hns/components/hns_news/browser/publishers_controller.h"
#include "hns/components/hns_news/common/hns_news.mojom-forward.h"
#include "third_party/abseil-cpp/absl/types/optional.h"

namespace hns_news {

absl::optional<Publishers> ParseCombinedPublisherList(const base::Value& value);

void ParseDirectPublisherList(const base::Value::Dict& direct_feeds_pref_dict,
                              std::vector<mojom::PublisherPtr>* publishers);

}  // namespace hns_news

#endif  // HNS_COMPONENTS_HNS_NEWS_BROWSER_PUBLISHERS_PARSING_H_
