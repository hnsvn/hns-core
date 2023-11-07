/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_wallet/browser/bitcoin/bitcoin_tx_state_manager.h"

#include <utility>

#include "base/notreached.h"
#include "base/values.h"
#include "hns/components/hns_wallet/browser/hns_wallet_constants.h"
#include "hns/components/hns_wallet/browser/hns_wallet_utils.h"
#include "hns/components/hns_wallet/browser/json_rpc_service.h"
#include "hns/components/hns_wallet/browser/tx_meta.h"

namespace hns_wallet {

BitcoinTxStateManager::BitcoinTxStateManager(
    PrefService* prefs,
    TxStorageDelegate* delegate,
    JsonRpcService* json_rpc_service,
    AccountResolverDelegate* account_resolver_delegate)
    : TxStateManager(prefs, delegate, account_resolver_delegate) {}

BitcoinTxStateManager::~BitcoinTxStateManager() = default;

// TODO(apaymyshev): test that
std::string BitcoinTxStateManager::GetTxPrefPathPrefix(
    const absl::optional<std::string>& chain_id) {
  if (chain_id.has_value()) {
    return base::StrCat(
        {kBitcoinPrefKey, ".",
         GetNetworkId(prefs_, mojom::CoinType::BTC, *chain_id)});
  }
  return kBitcoinPrefKey;
}

mojom::CoinType BitcoinTxStateManager::GetCoinType() const {
  return mojom::CoinType::BTC;
}

std::unique_ptr<TxMeta> BitcoinTxStateManager::ValueToTxMeta(
    const base::Value::Dict& value) {
  // TODO(apaymyshev): implement
  NOTIMPLEMENTED();

  return nullptr;
}

}  // namespace hns_wallet
