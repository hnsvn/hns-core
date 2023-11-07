/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/browser/net/hns_referrals_network_delegate_helper.h"

#include "hns/components/hns_referrals/browser/hns_referrals_service.h"
#include "hns/components/constants/network_constants.h"
#include "chrome/browser/browser_process.h"
#include "content/public/browser/browser_thread.h"
#include "extensions/common/url_pattern.h"
#include "net/url_request/url_request.h"

namespace hns {

int OnBeforeStartTransaction_ReferralsWork(
    net::HttpRequestHeaders* headers,
    const ResponseCallback& next_callback,
    std::shared_ptr<HnsRequestInfo> ctx) {
  // If the domain for this request matches one of our target domains,
  // set the associated custom headers.
  const base::Value::Dict* request_headers_dict = nullptr;
  if (!HnsReferralsHeaders::GetInstance()->GetMatchingReferralHeaders(
          &request_headers_dict, ctx->request_url))
    return net::OK;
  for (const auto it : *request_headers_dict) {
    if (it.first == kHnsPartnerHeader) {
      headers->SetHeader(it.first, it.second.GetString());
      ctx->set_headers.insert(it.first);
    }
  }
  return net::OK;
}

}  // namespace hns
