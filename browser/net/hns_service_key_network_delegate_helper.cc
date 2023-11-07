/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/browser/net/hns_service_key_network_delegate_helper.h"

#include <string>
#include <vector>

#include "base/no_destructor.h"
#include "hns/components/constants/hns_services_key.h"
#include "hns/components/constants/network_constants.h"
#include "hns/components/update_client/buildflags.h"
#include "net/base/net_errors.h"
#include "net/http/http_request_headers.h"
#include "url/gurl.h"

namespace hns {

int OnBeforeStartTransaction_HnsServiceKey(
    net::HttpRequestHeaders* headers,
    const ResponseCallback& next_callback,
    std::shared_ptr<HnsRequestInfo> ctx) {
  static const base::NoDestructor<std::vector<std::string>> allowed_domains{
      {kExtensionUpdaterDomain, GURL(BUILDFLAG(UPDATER_DEV_ENDPOINT)).host(),
       GURL(BUILDFLAG(UPDATER_PROD_ENDPOINT)).host()}};

  const GURL& url = ctx->request_url;

  if (url.SchemeIs(url::kHttpsScheme)) {
    if (std::any_of(
            allowed_domains->begin(), allowed_domains->end(),
            [&url](const auto& domain) { return url.DomainIs(domain); })) {
      headers->SetHeader(kHnsServicesKeyHeader,
                         BUILDFLAG(HNS_SERVICES_KEY));
    }
  }
  return net::OK;
}

}  // namespace hns
