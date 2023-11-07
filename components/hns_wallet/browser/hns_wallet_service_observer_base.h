/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_WALLET_BROWSER_HNS_WALLET_SERVICE_OBSERVER_BASE_H_
#define HNS_COMPONENTS_HNS_WALLET_BROWSER_HNS_WALLET_SERVICE_OBSERVER_BASE_H_

#include <string>
#include <vector>

#include "hns/components/hns_wallet/common/hns_wallet.mojom.h"

namespace hns_wallet {

class HnsWalletServiceObserverBase
    : public hns_wallet::mojom::HnsWalletServiceObserver {
  void OnActiveOriginChanged(mojom::OriginInfoPtr origin_info) override {}
  void OnDefaultEthereumWalletChanged(
      hns_wallet::mojom::DefaultWallet wallet) override {}
  void OnDefaultSolanaWalletChanged(
      hns_wallet::mojom::DefaultWallet wallet) override {}
  void OnDefaultBaseCurrencyChanged(const std::string& currency) override {}
  void OnDefaultBaseCryptocurrencyChanged(
      const std::string& cryptocurrency) override {}
  void OnNetworkListChanged() override {}
  void OnDiscoverAssetsStarted() override {}
  void OnDiscoverAssetsCompleted(
      std::vector<mojom::BlockchainTokenPtr> discovered_assets) override {}
  void OnResetWallet() override {}
};

}  // namespace hns_wallet

#endif  // HNS_COMPONENTS_HNS_WALLET_BROWSER_HNS_WALLET_SERVICE_OBSERVER_BASE_H_
