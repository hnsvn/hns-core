
/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/common/url/url_request_string_util.h"

#include <sstream>
#include <vector>

#include "base/ranges/algorithm.h"
#include "base/strings/strcat.h"
#include "base/strings/string_util.h"
#include "hns/components/hns_ads/core/mojom/hns_ads.mojom.h"

namespace hns_ads {

namespace {

bool ShouldAllowHeader(const std::string& header) {
  const std::vector<std::string> allowed_headers{"digest", "signature",
                                                 "accept", "content-type"};

  return base::ranges::any_of(
      allowed_headers, [&header](const std::string& allowed_header) {
        return base::StartsWith(header, allowed_header,
                                base::CompareCase::INSENSITIVE_ASCII);
      });
}

std::string HeadersToString(const std::vector<std::string>& headers,
                            const int indent = 4) {
  std::vector<std::string> formatted_headers;

  const std::string spaces = std::string(indent, ' ');

  for (const auto& header : headers) {
    if (!ShouldAllowHeader(header)) {
      continue;
    }

    const std::string formatted_header = base::StrCat({spaces, header});
    formatted_headers.push_back(formatted_header);
  }

  return base::JoinString(formatted_headers, "\n");
}

}  // namespace

std::string UrlRequestToString(const mojom::UrlRequestInfoPtr& url_request) {
  std::string log = "URL Request:\n";

  log += base::ReplaceStringPlaceholders("  URL: $1\n",
                                         {url_request->url.spec()}, nullptr);

  if (!url_request->content.empty()) {
    log += base::ReplaceStringPlaceholders("  Content: $1\n",
                                           {url_request->content}, nullptr);
  }

  if (!url_request->content_type.empty()) {
    log += base::ReplaceStringPlaceholders(
        "  Content Type: $1\n", {url_request->content_type}, nullptr);
  }

  std::ostringstream ss;
  ss << url_request->method;

  log += base::StrCat({"  Method: ", ss.str()});

  return log;
}

std::string UrlRequestHeadersToString(
    const mojom::UrlRequestInfoPtr& url_request) {
  std::string log = "  Headers:\n";

  if (!url_request->headers.empty()) {
    log += HeadersToString(url_request->headers);
  }

  return log;
}

}  // namespace hns_ads
