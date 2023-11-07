/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/common/search_engine/search_engine_info.h"

#include <utility>

namespace hns_ads {

SearchEngineInfo::SearchEngineInfo(std::string url_pattern,
                                   std::string result_page_url_pattern,
                                   std::string search_term_query_key)
    : url_pattern(std::move(url_pattern)),
      result_page_url_pattern(std::move(result_page_url_pattern)),
      search_term_query_key(std::move(search_term_query_key)) {}

}  // namespace hns_ads
