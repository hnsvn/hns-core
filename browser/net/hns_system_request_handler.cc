/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/browser/net/hns_system_request_handler.h"

#include "hns/browser/net/hns_block_safebrowsing_urls.h"
#include "hns/browser/net/hns_common_static_redirect_network_delegate_helper.h"
#include "hns/browser/net/hns_static_redirect_network_delegate_helper.h"
#include "hns/components/constants/hns_services_key.h"
#include "hns/components/constants/hns_services_key_helper.h"
#include "hns/components/constants/network_constants.h"
#include "extensions/common/url_pattern.h"
#include "services/network/public/cpp/resource_request.h"
#include "url/gurl.h"

namespace hns {

std::string HnsServicesKeyForTesting() {
  return BUILDFLAG(HNS_SERVICES_KEY);
}

void AddHnsServicesKeyHeader(network::ResourceRequest* url_request) {
  if (hns::ShouldAddHnsServicesKeyHeader(url_request->url)) {
    url_request->headers.SetHeaderIfMissing(kHnsServicesKeyHeader,
                                            BUILDFLAG(HNS_SERVICES_KEY));
  }
  return;
}

network::ResourceRequest OnBeforeSystemRequest(
    const network::ResourceRequest& url_request) {
  GURL new_url;
  hns::OnBeforeURLRequest_BlockSafeBrowsingReportingURLs(url_request.url,
                                                           &new_url);
  hns::OnBeforeURLRequest_StaticRedirectWorkForGURL(url_request.url,
                                                      &new_url);
  hns::OnBeforeURLRequest_CommonStaticRedirectWorkForGURL(url_request.url,
                                                            &new_url);
  network::ResourceRequest patched_request = url_request;
  if (!new_url.is_empty()) {
    patched_request.url = new_url;
  }
  AddHnsServicesKeyHeader(&patched_request);
  return patched_request;
}

}  // namespace hns
