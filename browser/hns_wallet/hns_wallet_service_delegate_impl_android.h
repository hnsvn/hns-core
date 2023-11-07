/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_HNS_WALLET_HNS_WALLET_SERVICE_DELEGATE_IMPL_ANDROID_H_
#define HNS_BROWSER_HNS_WALLET_HNS_WALLET_SERVICE_DELEGATE_IMPL_ANDROID_H_

#include <memory>
#include <string>

#include "base/memory/raw_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/observer_list.h"

#include "hns/components/hns_wallet/browser/hns_wallet_service_delegate.h"
#include "hns/components/hns_wallet/common/hns_wallet.mojom.h"

namespace content {
class BrowserContext;
}

namespace hns_wallet {

class ExternalWalletsImporter;

class HnsWalletServiceDelegateImpl : public HnsWalletServiceDelegate {
 public:
  explicit HnsWalletServiceDelegateImpl(content::BrowserContext* context);
  HnsWalletServiceDelegateImpl(const HnsWalletServiceDelegateImpl&) =
      delete;
  HnsWalletServiceDelegateImpl& operator=(
      const HnsWalletServiceDelegateImpl&) = delete;
  ~HnsWalletServiceDelegateImpl() override;

  bool AddPermission(mojom::CoinType coin,
                     const url::Origin& origin,
                     const std::string& account) override;
  bool HasPermission(mojom::CoinType coin,
                     const url::Origin& origin,
                     const std::string& account) override;
  bool ResetPermission(mojom::CoinType coin,
                       const url::Origin& origin,
                       const std::string& account) override;
  bool IsPermissionDenied(mojom::CoinType coin,
                          const url::Origin& origin) override;
  void GetWebSitesWithPermission(
      mojom::CoinType coin,
      GetWebSitesWithPermissionCallback callback) override;
  void ResetWebSitePermission(mojom::CoinType coin,
                              const std::string& formed_website,
                              ResetWebSitePermissionCallback callback) override;
  absl::optional<url::Origin> GetActiveOrigin() override;

 private:
  raw_ptr<content::BrowserContext> context_ = nullptr;
  base::ObserverList<HnsWalletServiceDelegate::Observer> observer_list_;

  base::WeakPtrFactory<HnsWalletServiceDelegateImpl> weak_ptr_factory_;
};

}  // namespace hns_wallet

#endif  // HNS_BROWSER_HNS_WALLET_HNS_WALLET_SERVICE_DELEGATE_IMPL_ANDROID_H_
