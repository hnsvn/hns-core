/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_IOS_BROWSER_API_HNS_WALLET_HNS_WALLET_PROVIDER_DELEGATE_IOS_PRIVATE_H_
#define HNS_IOS_BROWSER_API_HNS_WALLET_HNS_WALLET_PROVIDER_DELEGATE_IOS_PRIVATE_H_

#include <string>
#include <vector>

#include "hns/components/hns_wallet/browser/hns_wallet_provider_delegate.h"

@protocol HnsWalletProviderDelegate;

namespace hns_wallet {

class HnsWalletProviderDelegateBridge
    : public hns_wallet::HnsWalletProviderDelegate {
 public:
  explicit HnsWalletProviderDelegateBridge(
      id<HnsWalletProviderDelegate> bridge)
      : bridge_(bridge) {}

 private:
  __weak id<HnsWalletProviderDelegate> bridge_;

  bool IsTabVisible() override;
  void ShowPanel() override;
  void WalletInteractionDetected() override;
  url::Origin GetOrigin() const override;
  void ShowWalletOnboarding() override;
  void ShowAccountCreation(mojom::CoinType type) override;
  void RequestPermissions(mojom::CoinType type,
                          const std::vector<std::string>& accounts,
                          RequestPermissionsCallback) override;
  bool IsAccountAllowed(mojom::CoinType type,
                        const std::string& account) override;
  absl::optional<std::vector<std::string>> GetAllowedAccounts(
      mojom::CoinType type,
      const std::vector<std::string>& accounts) override;
  bool IsPermissionDenied(mojom::CoinType type) override;
  void AddSolanaConnectedAccount(const std::string& account) override;
  void RemoveSolanaConnectedAccount(const std::string& account) override;
  bool IsSolanaAccountConnected(const std::string& account) override;
};

}  // namespace hns_wallet

#endif  // HNS_IOS_BROWSER_API_HNS_WALLET_HNS_WALLET_PROVIDER_DELEGATE_IOS_PRIVATE_H_
