/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_WALLET_BROWSER_SOLANA_INSTRUCTION_BUILDER_H_
#define HNS_COMPONENTS_HNS_WALLET_BROWSER_SOLANA_INSTRUCTION_BUILDER_H_

#include <string>
#include <vector>

#include "third_party/abseil-cpp/absl/types/optional.h"

namespace hns_wallet {

class SolanaInstruction;

namespace solana {

namespace system_program {

absl::optional<SolanaInstruction> Transfer(const std::string& from_pubkey,
                                           const std::string& to_pubkey,
                                           uint64_t lamport);

}  // namespace system_program

namespace spl_token_program {

absl::optional<SolanaInstruction> Transfer(
    const std::string& token_program_id,
    const std::string& source_pubkey,
    const std::string& destination_pubkey,
    const std::string& authority_pubkey,
    const std::vector<std::string>& signer_pubkeys,
    uint64_t amount);

}  // namespace spl_token_program

namespace spl_associated_token_account_program {

absl::optional<SolanaInstruction> CreateAssociatedTokenAccount(
    const std::string& funding_address,
    const std::string& wallet_address,
    const std::string& associated_token_account_address,
    const std::string& spl_token_mint_address);

}  // namespace spl_associated_token_account_program

}  // namespace solana

}  // namespace hns_wallet

#endif  // HNS_COMPONENTS_HNS_WALLET_BROWSER_SOLANA_INSTRUCTION_BUILDER_H_
