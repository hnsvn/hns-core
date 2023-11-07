/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_CHROMIUM_SRC_CHROME_BROWSER_PERMISSIONS_PERMISSION_MANAGER_FACTORY_H_
#define HNS_CHROMIUM_SRC_CHROME_BROWSER_PERMISSIONS_PERMISSION_MANAGER_FACTORY_H_

#include "components/keyed_service/content/browser_context_keyed_service_factory.h"

namespace hns_wallet {
class EthereumProviderImplUnitTest;
class SolanaProviderImplUnitTest;
}  // namespace hns_wallet

namespace permissions {
class HnsWalletPermissionContextUnitTest;
}

#define BuildServiceInstanceForBrowserContext               \
  BuildServiceInstanceForBrowserContext_ChromiumImpl(       \
      content::BrowserContext* profile) const;              \
  friend hns_wallet::EthereumProviderImplUnitTest;        \
  friend hns_wallet::SolanaProviderImplUnitTest;          \
  friend permissions::HnsWalletPermissionContextUnitTest; \
  std::unique_ptr<KeyedService> BuildServiceInstanceForBrowserContext

#include "src/chrome/browser/permissions/permission_manager_factory.h"  // IWYU pragma: export
#undef BuildServiceInstanceForBrowserContext

#endif  // HNS_CHROMIUM_SRC_CHROME_BROWSER_PERMISSIONS_PERMISSION_MANAGER_FACTORY_H_
