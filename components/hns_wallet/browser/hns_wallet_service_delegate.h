/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_WALLET_BROWSER_HNS_WALLET_SERVICE_DELEGATE_H_
#define HNS_COMPONENTS_HNS_WALLET_BROWSER_HNS_WALLET_SERVICE_DELEGATE_H_

#include <memory>
#include <string>

#include "base/functional/callback.h"
#include "base/observer_list_types.h"
#include "hns/components/hns_wallet/common/hns_wallet.mojom.h"
#include "hns/components/hns_wallet/common/hns_wallet_types.h"
#include "third_party/abseil-cpp/absl/types/optional.h"
#include "url/origin.h"

namespace content {
class BrowserContext;
}

namespace hns_wallet {

class HnsWalletServiceDelegate {
 public:
  using IsExternalWalletInstalledCallback = base::OnceCallback<void(bool)>;
  using IsExternalWalletInitializedCallback = base::OnceCallback<void(bool)>;
  using GetImportInfoCallback =
      base::OnceCallback<void(bool, ImportInfo, ImportError)>;
  using GetWebSitesWithPermissionCallback =
      mojom::HnsWalletService::GetWebSitesWithPermissionCallback;
  using ResetWebSitePermissionCallback =
      mojom::HnsWalletService::ResetWebSitePermissionCallback;

  HnsWalletServiceDelegate() = default;
  HnsWalletServiceDelegate(const HnsWalletServiceDelegate&) = delete;
  HnsWalletServiceDelegate& operator=(const HnsWalletServiceDelegate&) =
      delete;
  virtual ~HnsWalletServiceDelegate() = default;

  class Observer : public base::CheckedObserver {
   public:
    virtual void OnActiveOriginChanged(
        const mojom::OriginInfoPtr& origin_info) {}
  };
  virtual void AddObserver(Observer* observer) {}
  virtual void RemoveObserver(Observer* observer) {}

  virtual void IsExternalWalletInstalled(mojom::ExternalWalletType,
                                         IsExternalWalletInstalledCallback);
  virtual void IsExternalWalletInitialized(mojom::ExternalWalletType,
                                           IsExternalWalletInitializedCallback);
  virtual void GetImportInfoFromExternalWallet(mojom::ExternalWalletType type,
                                               const std::string& password,
                                               GetImportInfoCallback callback);
  virtual bool AddPermission(mojom::CoinType coin,
                             const url::Origin& origin,
                             const std::string& account);
  virtual bool HasPermission(mojom::CoinType coin,
                             const url::Origin& origin,
                             const std::string& account);
  virtual bool ResetPermission(mojom::CoinType coin,
                               const url::Origin& origin,
                               const std::string& account);
  virtual bool IsPermissionDenied(mojom::CoinType coin,
                                  const url::Origin& origin);
  virtual void GetWebSitesWithPermission(
      mojom::CoinType coin,
      GetWebSitesWithPermissionCallback callback);
  virtual void ResetWebSitePermission(mojom::CoinType coin,
                                      const std::string& formed_website,
                                      ResetWebSitePermissionCallback callback);

  virtual absl::optional<url::Origin> GetActiveOrigin();

  virtual void ClearWalletUIStoragePartition();

  static std::unique_ptr<HnsWalletServiceDelegate> Create(
      content::BrowserContext* browser_context);
};

}  // namespace hns_wallet

#endif  // HNS_COMPONENTS_HNS_WALLET_BROWSER_HNS_WALLET_SERVICE_DELEGATE_H_
