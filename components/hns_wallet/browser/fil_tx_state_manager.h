/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_WALLET_BROWSER_FIL_TX_STATE_MANAGER_H_
#define HNS_COMPONENTS_HNS_WALLET_BROWSER_FIL_TX_STATE_MANAGER_H_

#include <memory>
#include <string>
#include <utility>

#include "base/gtest_prod_util.h"
#include "hns/components/hns_wallet/browser/tx_state_manager.h"

class PrefService;

namespace base {
class Value;
}  // namespace base

namespace hns_wallet {

class TxMeta;
class TxStorageDelegate;
class FilTxMeta;

class FilTxStateManager : public TxStateManager {
 public:
  FilTxStateManager(PrefService* prefs,
                    TxStorageDelegate* delegate,
                    AccountResolverDelegate* account_resolver_delegate);
  ~FilTxStateManager() override;
  FilTxStateManager(const FilTxStateManager&) = delete;
  FilTxStateManager operator=(const FilTxStateManager&) = delete;

  std::unique_ptr<FilTxMeta> GetFilTx(const std::string& chain_id,
                                      const std::string& id);
  std::unique_ptr<FilTxMeta> ValueToFilTxMeta(const base::Value::Dict& value);

 private:
  FRIEND_TEST_ALL_PREFIXES(FilTxStateManagerUnitTest, GetTxPrefPathPrefix);

  mojom::CoinType GetCoinType() const override;

  std::unique_ptr<TxMeta> ValueToTxMeta(
      const base::Value::Dict& value) override;
  std::string GetTxPrefPathPrefix(
      const absl::optional<std::string>& chain_id) override;
};

}  // namespace hns_wallet

#endif  // HNS_COMPONENTS_HNS_WALLET_BROWSER_FIL_TX_STATE_MANAGER_H_
