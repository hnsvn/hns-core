/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_EXTENSIONS_HNS_EXTENSIONS_BROWSER_API_PROVIDER_H_
#define HNS_BROWSER_EXTENSIONS_HNS_EXTENSIONS_BROWSER_API_PROVIDER_H_

#include "extensions/browser/extensions_browser_api_provider.h"

namespace extensions {

class HnsExtensionsBrowserAPIProvider : public ExtensionsBrowserAPIProvider {
 public:
  HnsExtensionsBrowserAPIProvider();
  HnsExtensionsBrowserAPIProvider(const HnsExtensionsBrowserAPIProvider&) =
      delete;
  HnsExtensionsBrowserAPIProvider& operator=(
      const HnsExtensionsBrowserAPIProvider&) = delete;
  ~HnsExtensionsBrowserAPIProvider() override;

  void RegisterExtensionFunctions(ExtensionFunctionRegistry* registry) override;
};

}  // namespace extensions

#endif  // HNS_BROWSER_EXTENSIONS_HNS_EXTENSIONS_BROWSER_API_PROVIDER_H_
