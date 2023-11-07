/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "ios/chrome/browser/net/ios_chrome_network_delegate.h"

#define IOSChromeNetworkDelegate IOSChromeNetworkDelegate_ChromiumImpl
#include "src/ios/chrome/browser/net/ios_chrome_network_delegate.cc"
#undef IOSChromeNetworkDelegate

#include "hns/components/constants/hns_services_key.h"
#include "hns/components/constants/hns_services_key_helper.h"
#include "hns/components/constants/network_constants.h"

namespace {

void AddHnsServicesKeyHeader(net::URLRequest* request) {
  if (hns::ShouldAddHnsServicesKeyHeader(request->url())) {
    request->SetExtraRequestHeaderByName(kHnsServicesKeyHeader,
                                         BUILDFLAG(HNS_SERVICES_KEY),
                                         true /* overrwrite */);
  }
}

}  // namespace

IOSChromeNetworkDelegate::~IOSChromeNetworkDelegate() = default;

int IOSChromeNetworkDelegate::OnBeforeURLRequest(
    net::URLRequest* request,
    net::CompletionOnceCallback callback,
    GURL* new_url) {
  const auto result = IOSChromeNetworkDelegate_ChromiumImpl::OnBeforeURLRequest(
      request, std::move(callback), new_url);
  ::AddHnsServicesKeyHeader(request);
  return result;
}
