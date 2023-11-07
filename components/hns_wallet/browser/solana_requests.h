/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_WALLET_BROWSER_SOLANA_REQUESTS_H_
#define HNS_COMPONENTS_HNS_WALLET_BROWSER_SOLANA_REQUESTS_H_

#include <string>
#include <vector>

#include "hns/components/hns_wallet/browser/solana_transaction.h"
#include "third_party/abseil-cpp/absl/types/optional.h"

namespace hns_wallet {

namespace solana {

std::string getBalance(const std::string& pubkey);
std::string getTokenAccountBalance(const std::string& pubkey);
std::string sendTransaction(
    const std::string& signed_tx,
    absl::optional<SolanaTransaction::SendOptions> options);
std::string getLatestBlockhash();
std::string getSignatureStatuses(const std::vector<std::string>& tx_signatures);
std::string getAccountInfo(const std::string& pubkey);
std::string getFeeForMessage(const std::string& message);
std::string getBlockHeight();
std::string getTokenAccountsByOwner(const std::string& pubkey,
                                    const std::string& encoding);
std::string isBlockhashValid(const std::string& blockhash,
                             const absl::optional<std::string>& commitment);

}  // namespace solana

}  // namespace hns_wallet

#endif  // HNS_COMPONENTS_HNS_WALLET_BROWSER_SOLANA_REQUESTS_H_
