/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/common/url/url_response_string_util.h"

#include <vector>

#include "base/containers/flat_map.h"
#include "base/strings/strcat.h"
#include "base/strings/string_util.h"
#include "base/strings/stringprintf.h"
#include "hns/components/hns_ads/core/mojom/hns_ads.mojom.h"

namespace hns_ads {

namespace {

std::string HeadersToString(
    const base::flat_map<std::string, std::string>& headers,
    const int indent = 4) {
  std::vector<std::string> formatted_headers;

  const std::string spaces = std::string(indent, ' ');

  for (const auto& [header, value] : headers) {
    const std::string formatted_header = base::ReplaceStringPlaceholders(
        "$1$2: $3", {spaces, header, value}, nullptr);

    formatted_headers.push_back(formatted_header);
  }

  return base::JoinString(formatted_headers, "\n");
}

}  // namespace

std::string UrlResponseToString(const mojom::UrlResponseInfo& url_response) {
  return base::StringPrintf(
      "URL Response:\n  URL: %s\n  Response "
      "Status Code: %d\n  Response: %s",
      url_response.url.spec().c_str(), url_response.status_code,
      url_response.body.c_str());
}

std::string UrlResponseHeadersToString(
    const mojom::UrlResponseInfo& url_response) {
  return base::StrCat({"  Headers:\n", HeadersToString(url_response.headers)});
}

}  // namespace hns_ads
