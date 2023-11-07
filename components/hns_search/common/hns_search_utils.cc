// Copyright (c) 2021 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

#include "hns/components/hns_search/common/hns_search_utils.h"

#include <string>
#include <vector>

#include "base/containers/contains.h"
#include "base/containers/fixed_flat_set.h"
#include "base/feature_list.h"
#include "base/strings/string_piece_forward.h"
#include "hns/components/hns_search/common/features.h"
#include "url/gurl.h"
#include "url/url_constants.h"

namespace {

constexpr auto kVettedHosts = base::MakeFixedFlatSet<base::StringPiece>(
    {"search.hns.com", "search.hns.software", "search.hnssoftware.com",
     "safesearch.hns.com", "safesearch.hns.software",
     "safesearch.hnssoftware.com", "search-dev-local.hns.com"});

}  // namespace

namespace hns_search {

bool IsAllowedHost(const GURL& url) {
  if (!url.is_valid() || !url.SchemeIs(url::kHttpsScheme)) {
    return false;
  }
  std::string host = url.host();
  return base::Contains(kVettedHosts, host);
}

bool IsDefaultAPIEnabled() {
  return base::FeatureList::IsEnabled(
      hns_search::features::kHnsSearchDefaultAPIFeature);
}

}  // namespace hns_search
