/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_WALLET_BROWSER_SOLANA_INSTRUCTION_DATA_DECODER_H_
#define HNS_COMPONENTS_HNS_WALLET_BROWSER_SOLANA_INSTRUCTION_DATA_DECODER_H_

#include <string>
#include <vector>

#include "hns/components/hns_wallet/browser/solana_instruction_decoded_data.h"
#include "hns/components/hns_wallet/common/hns_wallet.mojom.h"
#include "third_party/abseil-cpp/absl/types/optional.h"

namespace hns_wallet::solana_ins_data_decoder {

absl::optional<SolanaInstructionDecodedData> Decode(
    const std::vector<uint8_t>& data,
    const std::string& program_id);

std::vector<InsParamPair> GetAccountParamsForTesting(
    absl::optional<mojom::SolanaSystemInstruction> sys_ins_type,
    absl::optional<mojom::SolanaTokenInstruction> token_ins_type);

std::vector<mojom::SolanaInstructionAccountParamPtr>
GetMojomAccountParamsForTesting(
    absl::optional<mojom::SolanaSystemInstruction> sys_ins_type,
    absl::optional<mojom::SolanaTokenInstruction> token_ins_type);

}  // namespace hns_wallet::solana_ins_data_decoder

#endif  // HNS_COMPONENTS_HNS_WALLET_BROWSER_SOLANA_INSTRUCTION_DATA_DECODER_H_
