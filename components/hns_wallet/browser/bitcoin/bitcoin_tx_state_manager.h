/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_WALLET_BROWSER_BITCOIN_BITCOIN_TX_STATE_MANAGER_H_
#define HNS_COMPONENTS_HNS_WALLET_BROWSER_BITCOIN_BITCOIN_TX_STATE_MANAGER_H_

#include <memory>
#include <string>
#include <utility>

#include "hns/components/hns_wallet/browser/tx_state_manager.h"

class PrefService;

namespace base {
class Value;
}  // namespace base

namespace hns_wallet {

class TxMeta;
class TxStorageDelegate;
class JsonRpcService;

class BitcoinTxStateManager : public TxStateManager {
 public:
  BitcoinTxStateManager(PrefService* prefs,
                        TxStorageDelegate* delegate,
                        JsonRpcService* json_rpc_service,
                        AccountResolverDelegate* account_resolver_delegate);
  ~BitcoinTxStateManager() override;
  BitcoinTxStateManager(const BitcoinTxStateManager&) = delete;
  BitcoinTxStateManager operator=(const BitcoinTxStateManager&) = delete;

 private:
  mojom::CoinType GetCoinType() const override;

  std::unique_ptr<TxMeta> ValueToTxMeta(
      const base::Value::Dict& value) override;
  std::string GetTxPrefPathPrefix(
      const absl::optional<std::string>& chain_id) override;
};

}  // namespace hns_wallet

#endif  // HNS_COMPONENTS_HNS_WALLET_BROWSER_BITCOIN_BITCOIN_TX_STATE_MANAGER_H_
