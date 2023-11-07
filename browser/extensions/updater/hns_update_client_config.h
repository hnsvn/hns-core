// Copyright (c) 2019 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef HNS_BROWSER_EXTENSIONS_UPDATER_HNS_UPDATE_CLIENT_CONFIG_H_
#define HNS_BROWSER_EXTENSIONS_UPDATER_HNS_UPDATE_CLIENT_CONFIG_H_

#include <string>

#include "base/memory/ref_counted.h"
#include "base/memory/scoped_refptr.h"
#include "chrome/browser/extensions/updater/chrome_update_client_config.h"
#include "third_party/abseil-cpp/absl/types/optional.h"

class GURL;

namespace content {
class BrowserContext;
}

namespace update_client {
class NetworkFetcherFactory;
}

namespace extensions {

class ExtensionUpdateClientBaseTest;

class HnsUpdateClientConfig : public ChromeUpdateClientConfig {
 public:
  HnsUpdateClientConfig(const HnsUpdateClientConfig&) = delete;
  HnsUpdateClientConfig& operator=(const HnsUpdateClientConfig&) = delete;

  static scoped_refptr<ChromeUpdateClientConfig> Create(
      content::BrowserContext* context,
      absl::optional<GURL> url_override);

  using ChromeUpdateClientConfig::ChromeUpdateClientConfig;

  std::string GetChannel() const override;
  scoped_refptr<update_client::NetworkFetcherFactory> GetNetworkFetcherFactory()
      override;
  bool EnabledCupSigning() const override;

 protected:
  friend class base::RefCountedThreadSafe<HnsUpdateClientConfig>;
  friend class ExtensionUpdateClientBaseTest;

  ~HnsUpdateClientConfig() override;
};

}  // namespace extensions

#endif  // HNS_BROWSER_EXTENSIONS_UPDATER_HNS_UPDATE_CLIENT_CONFIG_H_
