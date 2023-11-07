/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_EXTENSIONS_HNS_EXTENSIONS_BROWSER_CLIENT_IMPL_H_
#define HNS_BROWSER_EXTENSIONS_HNS_EXTENSIONS_BROWSER_CLIENT_IMPL_H_

#include "chrome/browser/extensions/chrome_extensions_browser_client.h"

namespace extensions {

class HnsExtensionsBrowserClientImpl : public ChromeExtensionsBrowserClient {
 public:
  HnsExtensionsBrowserClientImpl();
  HnsExtensionsBrowserClientImpl(const HnsExtensionsBrowserClientImpl&) =
      delete;
  HnsExtensionsBrowserClientImpl& operator=(
      const HnsExtensionsBrowserClientImpl&) = delete;
  ~HnsExtensionsBrowserClientImpl() override = default;
};

}  // namespace extensions

#endif  // HNS_BROWSER_EXTENSIONS_HNS_EXTENSIONS_BROWSER_CLIENT_IMPL_H_
