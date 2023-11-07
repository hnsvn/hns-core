// Copyright (c) 2019 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

#include "hns/browser/extensions/updater/hns_update_client_config.h"

#include <string>

#include "base/functional/bind.h"
#include "base/memory/scoped_refptr.h"
#include "base/no_destructor.h"
#include "components/update_client/net/network_chromium.h"
#include "content/public/browser/browser_context.h"
#include "content/public/browser/storage_partition.h"
#include "url/gurl.h"


namespace extensions {

namespace {

using FactoryCallback = ChromeUpdateClientConfig::FactoryCallback;

// static
static FactoryCallback& GetFactoryCallback() {
  static base::NoDestructor<FactoryCallback> factory;
  return *factory;
}

}  // namespace

std::string HnsUpdateClientConfig::GetChannel() const {
  return std::string("stable");
}

scoped_refptr<update_client::NetworkFetcherFactory>
HnsUpdateClientConfig::GetNetworkFetcherFactory() {
  if (!network_fetcher_factory_) {
    network_fetcher_factory_ =
        base::MakeRefCounted<update_client::NetworkFetcherChromiumFactory>(
            context_->GetDefaultStoragePartition()
                ->GetURLLoaderFactoryForBrowserProcess(),
            // Unlike ChromeUpdateClientConfig, which allows to send cookies for
            // chrome.google.com, we won't be sending any cookies.
            base::BindRepeating([](const GURL& url) { return false; }));
  }
  return network_fetcher_factory_;
}

// Disabling cup signing
bool HnsUpdateClientConfig::EnabledCupSigning() const {
  return false;
}

HnsUpdateClientConfig::~HnsUpdateClientConfig() = default;

// static
scoped_refptr<ChromeUpdateClientConfig> HnsUpdateClientConfig::Create(
    content::BrowserContext* context,
    absl::optional<GURL> update_url_override) {
  FactoryCallback& factory = GetFactoryCallback();
  return factory.is_null() ? base::MakeRefCounted<HnsUpdateClientConfig>(
                                 context, update_url_override)
                           : factory.Run(context);
}

}  // namespace extensions
