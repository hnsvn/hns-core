/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_WALLET_COMMON_HASH_UTILS_H_
#define HNS_COMPONENTS_HNS_WALLET_COMMON_HASH_UTILS_H_

#include <array>
#include <string>
#include <vector>

#include "hns/components/hns_wallet/common/eth_abi_utils.h"
#include "crypto/sha2.h"

namespace hns_wallet {

// Equivalent to web3.utils.keccak256(string)
std::string KeccakHash(const std::string& input, bool to_hex = true);
std::vector<uint8_t> KeccakHash(const std::vector<uint8_t>& input);
eth_abi::Bytes32 KeccakHashBytes32(base::span<const uint8_t> input);

// Returns the hex encoding of the first 4 bytes of the hash.
// For example: keccak('balanceOf(address)')
std::string GetFunctionHash(const std::string& input);
eth_abi::Bytes4 GetFunctionHashBytes4(const std::string& input);

// Implement namehash algorithm based on EIP-137 spec.
// Used for converting domain names in the classic format (ex: hns.crypto) to
// an ERC-721 token for ENS and Unstoppable Domains.
eth_abi::Bytes32 Namehash(const std::string& name);

// sha256(sha256(input))
std::array<uint8_t, crypto::kSHA256Length> DoubleSHA256Hash(
    base::span<const uint8_t> input);

// ripemd160(sha256(input))
std::vector<uint8_t> Hash160(base::span<const uint8_t> input);

}  // namespace hns_wallet

#endif  // HNS_COMPONENTS_HNS_WALLET_COMMON_HASH_UTILS_H_
