/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_CHROMIUM_SRC_NET_URL_REQUEST_REDIRECT_UTIL_H_
#define HNS_CHROMIUM_SRC_NET_URL_REQUEST_REDIRECT_UTIL_H_

#define UpdateHttpRequest                                              \
  UpdateHttpRequest_ChromiumImpl(                                      \
      const GURL& original_url, const std::string& original_method,    \
      const RedirectInfo& redirect_info,                               \
      const absl::optional<std::vector<std::string>>& removed_headers, \
      const absl::optional<net::HttpRequestHeaders>& modified_headers, \
      HttpRequestHeaders* request_headers, bool* should_clear_upload); \
  NET_EXPORT static void UpdateHttpRequest

#include "src/net/url_request/redirect_util.h"  // IWYU pragma: export
#undef UpdateHttpRequest

#endif  // HNS_CHROMIUM_SRC_NET_URL_REQUEST_REDIRECT_UTIL_H_
