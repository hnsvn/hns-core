/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_WALLET_BROWSER_UNSTOPPABLE_DOMAINS_DNS_RESOLVE_H_
#define HNS_COMPONENTS_HNS_WALLET_BROWSER_UNSTOPPABLE_DOMAINS_DNS_RESOLVE_H_

#include <string>
#include <vector>

#include "url/gurl.h"

namespace hns_wallet::unstoppable_domains {

const std::vector<std::string>& GetRecordKeys();
GURL ResolveUrl(const std::vector<std::string>& response);

}  // namespace hns_wallet::unstoppable_domains

#endif  // HNS_COMPONENTS_HNS_WALLET_BROWSER_UNSTOPPABLE_DOMAINS_DNS_RESOLVE_H_
