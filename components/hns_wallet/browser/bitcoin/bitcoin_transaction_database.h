/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_WALLET_BROWSER_BITCOIN_BITCOIN_TRANSACTION_DATABASE_H_
#define HNS_COMPONENTS_HNS_WALLET_BROWSER_BITCOIN_BITCOIN_TRANSACTION_DATABASE_H_

#include <map>
#include <set>
#include <string>
#include <vector>

#include "hns/components/hns_wallet/browser/bitcoin/bitcoin_transaction.h"

namespace hns_wallet {

// TODO(apaymyshev): tests for this class
class BitcoinTransactionDatabase {
 public:
  BitcoinTransactionDatabase();
  ~BitcoinTransactionDatabase();
  BitcoinTransactionDatabase(BitcoinTransactionDatabase& other) = delete;
  BitcoinTransactionDatabase& operator=(BitcoinTransactionDatabase& other) =
      delete;

  void SetChainHeight(uint32_t chain_height);
  absl::optional<uint32_t> GetChainHeight() const;
  bool AddTransactions(const std::string& address,
                       std::vector<bitcoin::Transaction> transactions);
  std::vector<bitcoin::Output> GetUnspentOutputs(const std::string& address);
  std::vector<bitcoin::Output> GetAllUnspentOutputs();
  uint64_t GetBalance(const std::string& address);

 private:
  absl::optional<uint32_t> chain_height_;
  // TODO(apaymyshev): avoid transaction duplicates for different addresses?
  std::map<std::string, std::set<bitcoin::Transaction>> transactions_;
};

}  // namespace hns_wallet

#endif  // HNS_COMPONENTS_HNS_WALLET_BROWSER_BITCOIN_BITCOIN_TRANSACTION_DATABASE_H_
