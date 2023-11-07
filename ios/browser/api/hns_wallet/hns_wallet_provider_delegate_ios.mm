/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/ios/browser/api/hns_wallet/hns_wallet_provider_delegate_ios.h"

#include "base/strings/sys_string_conversions.h"
#include "hns/base/mac/conversions.h"
#include "hns/components/hns_wallet/common/hns_wallet.mojom.h"
#include "hns/ios/browser/api/hns_wallet/hns_wallet_provider_delegate_ios+private.h"
#include "hns/ios/browser/api/url/url_origin_ios+private.h"
#include "net/base/mac/url_conversions.h"

namespace hns_wallet {

bool HnsWalletProviderDelegateBridge::IsTabVisible() {
  return [bridge_ isTabVisible];
}

void HnsWalletProviderDelegateBridge::ShowPanel() {
  [bridge_ showPanel];
}

void HnsWalletProviderDelegateBridge::WalletInteractionDetected() {
  [bridge_ walletInteractionDetected];
}

url::Origin HnsWalletProviderDelegateBridge::GetOrigin() const {
  return url::Origin([[bridge_ getOrigin] underlyingOrigin]);
}

void HnsWalletProviderDelegateBridge::ShowWalletOnboarding() {
  [bridge_ showWalletOnboarding];
}

void HnsWalletProviderDelegateBridge::ShowAccountCreation(
    mojom::CoinType type) {
  [bridge_ showAccountCreation:static_cast<HnsWalletCoinType>(type)];
}

void HnsWalletProviderDelegateBridge::RequestPermissions(
    mojom::CoinType type,
    const std::vector<std::string>& accounts,
    RequestPermissionsCallback callback) {
  auto completion = [callback = std::make_shared<decltype(callback)>(std::move(
                         callback))](HnsWalletRequestPermissionsError error,
                                     NSArray<NSString*>* _Nullable results) {
    if (!callback) {
      return;
    }
    if (results == nil) {
      std::move(*callback).Run(
          static_cast<mojom::RequestPermissionsError>(error), absl::nullopt);
      return;
    }
    std::vector<std::string> v;
    for (NSString* result in results) {
      v.push_back(base::SysNSStringToUTF8(result));
    }
    std::move(*callback).Run(static_cast<mojom::RequestPermissionsError>(error),
                             v);
  };
  [bridge_ requestPermissions:static_cast<HnsWalletCoinType>(type)
                     accounts:hns::vector_to_ns(accounts)
                   completion:completion];
}

bool HnsWalletProviderDelegateBridge::IsAccountAllowed(
    mojom::CoinType type,
    const std::string& account) {
  return [bridge_ isAccountAllowed:static_cast<HnsWalletCoinType>(type)
                           account:base::SysUTF8ToNSString(account)];
}

absl::optional<std::vector<std::string>>
HnsWalletProviderDelegateBridge::GetAllowedAccounts(
    mojom::CoinType type,
    const std::vector<std::string>& accounts) {
  NSArray<NSString*>* results =
      [bridge_ getAllowedAccounts:static_cast<HnsWalletCoinType>(type)
                         accounts:hns::vector_to_ns(accounts)];
  if (!results) {
    return absl::nullopt;
  }

  return hns::ns_to_vector<std::string>(results);
}

bool HnsWalletProviderDelegateBridge::IsPermissionDenied(
    mojom::CoinType type) {
  return [bridge_ isPermissionDenied:static_cast<HnsWalletCoinType>(type)];
}

void HnsWalletProviderDelegateBridge::AddSolanaConnectedAccount(
    const std::string& account) {
  [bridge_ addSolanaConnectedAccount:base::SysUTF8ToNSString(account)];
}

void HnsWalletProviderDelegateBridge::RemoveSolanaConnectedAccount(
    const std::string& account) {
  [bridge_ removeSolanaConnectedAccount:base::SysUTF8ToNSString(account)];
}

bool HnsWalletProviderDelegateBridge::IsSolanaAccountConnected(
    const std::string& account) {
  return [bridge_ isSolanaAccountConnected:base::SysUTF8ToNSString(account)];
}

}  // namespace hns_wallet
