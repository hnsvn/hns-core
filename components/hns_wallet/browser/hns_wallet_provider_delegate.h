/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_WALLET_BROWSER_HNS_WALLET_PROVIDER_DELEGATE_H_
#define HNS_COMPONENTS_HNS_WALLET_BROWSER_HNS_WALLET_PROVIDER_DELEGATE_H_

#include <string>
#include <vector>

#include "base/functional/callback.h"
#include "hns/components/hns_wallet/common/hns_wallet.mojom-forward.h"
#include "third_party/abseil-cpp/absl/types/optional.h"
#include "url/origin.h"

namespace hns_wallet {

class HnsWalletProviderDelegate {
 public:
  using RequestPermissionsCallback = base::OnceCallback<void(
      mojom::RequestPermissionsError error,
      const absl::optional<std::vector<std::string>>& allowed_accounts)>;

  HnsWalletProviderDelegate() = default;
  HnsWalletProviderDelegate(const HnsWalletProviderDelegate&) = delete;
  HnsWalletProviderDelegate& operator=(const HnsWalletProviderDelegate&) =
      delete;
  virtual ~HnsWalletProviderDelegate() = default;

  virtual bool IsTabVisible() = 0;
  virtual void ShowPanel() = 0;
  virtual void WalletInteractionDetected() = 0;
  virtual void ShowWalletOnboarding() = 0;
  virtual void ShowAccountCreation(mojom::CoinType type) = 0;
  virtual url::Origin GetOrigin() const = 0;
  virtual void RequestPermissions(mojom::CoinType type,
                                  const std::vector<std::string>& accounts,
                                  RequestPermissionsCallback) = 0;
  virtual bool IsAccountAllowed(mojom::CoinType type,
                                const std::string& account) = 0;
  virtual absl::optional<std::vector<std::string>> GetAllowedAccounts(
      mojom::CoinType type,
      const std::vector<std::string>& accounts) = 0;
  virtual bool IsPermissionDenied(mojom::CoinType type) = 0;
  virtual void AddSolanaConnectedAccount(const std::string& account) {}
  virtual void RemoveSolanaConnectedAccount(const std::string& account) {}
  virtual bool IsSolanaAccountConnected(const std::string& account);
};

}  // namespace hns_wallet

#endif  // HNS_COMPONENTS_HNS_WALLET_BROWSER_HNS_WALLET_PROVIDER_DELEGATE_H_
