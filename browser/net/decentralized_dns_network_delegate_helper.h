/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_NET_DECENTRALIZED_DNS_NETWORK_DELEGATE_HELPER_H_
#define HNS_BROWSER_NET_DECENTRALIZED_DNS_NETWORK_DELEGATE_HELPER_H_

#include <memory>
#include <string>
#include <vector>

#include "hns/browser/net/url_context.h"
#include "hns/components/hns_wallet/common/hns_wallet.mojom.h"
#include "net/base/completion_once_callback.h"

namespace decentralized_dns {

// Issue eth_call requests via Ethereum provider such as Infura to query
// decentralized DNS records, and redirect URL requests based on them.
int OnBeforeURLRequest_DecentralizedDnsPreRedirectWork(
    const hns::ResponseCallback& next_callback,
    std::shared_ptr<hns::HnsRequestInfo> ctx);

void OnBeforeURLRequest_UnstoppableDomainsRedirectWork(
    const hns::ResponseCallback& next_callback,
    std::shared_ptr<hns::HnsRequestInfo> ctx,
    const absl::optional<GURL>& url,
    hns_wallet::mojom::ProviderError error,
    const std::string& error_message);

void OnBeforeURLRequest_EnsRedirectWork(
    const hns::ResponseCallback& next_callback,
    std::shared_ptr<hns::HnsRequestInfo> ctx,
    const std::vector<uint8_t>& content_hash,
    bool require_offchain_consent,
    hns_wallet::mojom::ProviderError error,
    const std::string& error_message);

void OnBeforeURLRequest_SnsRedirectWork(
    const hns::ResponseCallback& next_callback,
    std::shared_ptr<hns::HnsRequestInfo> ctx,
    const absl::optional<GURL>& url,
    hns_wallet::mojom::SolanaProviderError error,
    const std::string& error_message);

}  // namespace decentralized_dns

#endif  // HNS_BROWSER_NET_DECENTRALIZED_DNS_NETWORK_DELEGATE_HELPER_H_
