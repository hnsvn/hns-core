/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/common/unittest/unittest_url_response_headers_util.h"

#include "base/check.h"
#include "base/strings/string_split.h"

namespace hns_ads {

base::flat_map<std::string, std::string> ToUrlResponseHeaders(
    const std::vector<std::string>& headers) {
  base::flat_map<std::string, std::string> response_headers;

  for (const auto& header : headers) {
    const std::vector<std::string> components = base::SplitString(
        header, ":", base::TRIM_WHITESPACE, base::SPLIT_WANT_ALL);
    CHECK(components.size() == 2) << "Invalid header: " << header;

    const std::string& name = components.at(0);
    const std::string& value = components.at(1);

    response_headers[name] = value;
  }

  return response_headers;
}

}  // namespace hns_ads
