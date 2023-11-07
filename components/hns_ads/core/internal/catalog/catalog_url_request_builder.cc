/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/catalog/catalog_url_request_builder.h"

#include <string>

#include "base/strings/stringprintf.h"
#include "hns/components/hns_ads/core/internal/catalog/catalog_url_request_builder_util.h"
#include "hns/components/hns_ads/core/internal/common/url/request_builder/host/url_host_util.h"
#include "hns/components/hns_ads/core/mojom/hns_ads.mojom.h"
#include "url/gurl.h"

namespace hns_ads {

namespace {

GURL BuildUrl() {
  const std::string spec = base::StringPrintf(
      "%s%s", GetStaticUrlHost().c_str(), BuildCatalogUrlPath().c_str());
  return GURL(spec);
}

}  // namespace

mojom::UrlRequestInfoPtr CatalogUrlRequestBuilder::Build() {
  mojom::UrlRequestInfoPtr url_request = mojom::UrlRequestInfo::New();
  url_request->url = BuildUrl();
  url_request->method = mojom::UrlRequestMethodType::kGet;

  return url_request;
}

}  // namespace hns_ads
