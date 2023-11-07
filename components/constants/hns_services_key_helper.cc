/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/constants/hns_services_key_helper.h"

#include "hns/components/constants/network_constants.h"
#include "extensions/common/url_pattern.h"
#include "url/gurl.h"

namespace hns {

bool ShouldAddHnsServicesKeyHeader(const GURL& url) {
  static URLPattern hns_proxy_pattern(URLPattern::SCHEME_HTTPS,
                                        kHnsProxyPattern);
  static URLPattern hnssoftware_proxy_pattern(URLPattern::SCHEME_HTTPS,
                                                kHnsSoftwareProxyPattern);
  return hns_proxy_pattern.MatchesURL(url) ||
         hnssoftware_proxy_pattern.MatchesURL(url);
}

}  // namespace hns
