/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/browser/net/hns_ads_status_header_network_delegate_helper.h"

#include <string>
#include <vector>

#include "hns/components/hns_rewards/common/pref_names.h"
#include "hns/components/hns_search/common/hns_search_utils.h"
#include "chrome/browser/profiles/profile.h"
#include "components/prefs/pref_service.h"
#include "content/public/browser/browser_context.h"
#include "net/base/net_errors.h"

namespace hns {

int OnBeforeStartTransaction_AdsStatusHeader(
    net::HttpRequestHeaders* headers,
    const ResponseCallback& next_callback,
    std::shared_ptr<HnsRequestInfo> ctx) {
  Profile* profile = Profile::FromBrowserContext(ctx->browser_context);

  // The X-Hns-Ads-Enabled header should be added when Hns Private Ads are
  // enabled, the requested URL host is one of the Hns Search domains, and the
  // request originates from one of the Hns Search domains.
  if (!profile->GetPrefs()->GetBoolean(hns_rewards::prefs::kEnabled) ||
      !hns_search::IsAllowedHost(ctx->request_url) ||
      (!hns_search::IsAllowedHost(ctx->tab_origin) &&
       !hns_search::IsAllowedHost(ctx->initiator_url))) {
    return net::OK;
  }

  headers->SetHeader(kAdsStatusHeader, kAdsEnabledStatusValue);
  ctx->set_headers.insert(kAdsStatusHeader);

  return net::OK;
}

}  // namespace hns
