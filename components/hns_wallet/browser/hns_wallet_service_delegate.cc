/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_wallet/browser/hns_wallet_service_delegate.h"

#include <utility>
#include <vector>

#include "base/notreached.h"

namespace hns_wallet {

void HnsWalletServiceDelegate::IsExternalWalletInstalled(
    mojom::ExternalWalletType type,
    IsExternalWalletInstalledCallback callback) {
  NOTIMPLEMENTED();
  std::move(callback).Run(false);
}

void HnsWalletServiceDelegate::IsExternalWalletInitialized(
    mojom::ExternalWalletType type,
    IsExternalWalletInitializedCallback callback) {
  NOTIMPLEMENTED();
  std::move(callback).Run(false);
}

void HnsWalletServiceDelegate::GetImportInfoFromExternalWallet(
    mojom::ExternalWalletType type,
    const std::string& password,
    GetImportInfoCallback callback) {
  NOTIMPLEMENTED();
  std::move(callback).Run(false, ImportInfo(), ImportError::kInternalError);
}

bool HnsWalletServiceDelegate::AddPermission(mojom::CoinType coin,
                                               const url::Origin& origin,
                                               const std::string& account) {
  NOTIMPLEMENTED();
  return false;
}

bool HnsWalletServiceDelegate::HasPermission(mojom::CoinType coin,
                                               const url::Origin& origin,
                                               const std::string& account) {
  NOTIMPLEMENTED();
  return false;
}

bool HnsWalletServiceDelegate::ResetPermission(mojom::CoinType coin,
                                                 const url::Origin& origin,
                                                 const std::string& account) {
  NOTIMPLEMENTED();
  return false;
}

bool HnsWalletServiceDelegate::IsPermissionDenied(mojom::CoinType coin,
                                                    const url::Origin& origin) {
  NOTIMPLEMENTED();
  return false;
}

absl::optional<url::Origin> HnsWalletServiceDelegate::GetActiveOrigin() {
  NOTIMPLEMENTED();
  return absl::nullopt;
}

void HnsWalletServiceDelegate::ClearWalletUIStoragePartition() {}

void HnsWalletServiceDelegate::GetWebSitesWithPermission(
    mojom::CoinType coin,
    GetWebSitesWithPermissionCallback callback) {
  NOTIMPLEMENTED();
  std::move(callback).Run(std::vector<std::string>());
}

void HnsWalletServiceDelegate::ResetWebSitePermission(
    mojom::CoinType coin,
    const std::string& formed_website,
    ResetWebSitePermissionCallback callback) {
  NOTIMPLEMENTED();
  std::move(callback).Run(false);
}

}  // namespace hns_wallet
