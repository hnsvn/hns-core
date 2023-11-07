// Copyright (c) 2022 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at https://mozilla.org/MPL/2.0/.

// types
import { HnsWallet } from '../constants/types'

// utils
import { getLocale } from '$web-common/locale'

/**
 * see: https://docs.solana.com/developing/runtime-facilities/programs
 * @param programId Solana Program public key
 * @returns Name of program
 */
export const getSolanaProgramIdName = (programId: string): string => {
  switch (programId) {
    case HnsWallet.SOLANA_SYSTEM_PROGRAM_ID:
      return getLocale('hnsWalletSolanaSystemProgram')
    case HnsWallet.SOLANA_CONFIG_PROGRAM_ID:
      return getLocale('hnsWalletSolanaConfigProgram')
    case HnsWallet.SOLANA_STAKE_PROGRAM_ID:
      return getLocale('hnsWalletSolanaStakeProgram')
    case HnsWallet.SOLANA_VOTE_PROGRAM_ID:
      return getLocale('hnsWalletSolanaVoteProgram')
    case HnsWallet.SOLANA_BPF_LOADER_UPGRADEABLE_PROGRAM_ID:
      return getLocale('hnsWalletSolanaBPFLoader')
    case HnsWallet.SOLANA_ED25519_SIG_VERIFY_PROGRAM_ID:
      return getLocale('hnsWalletSolanaEd25519Program')
    case HnsWallet.SOLANA_KECCAK_SECP256K_PROGRAM_ID:
      return getLocale('hnsWalletSolanaSecp256k1Program')
    case HnsWallet.SOLANA_ASSOCIATED_TOKEN_PROGRAM_ID:
      return getLocale('hnsWalletSolanaAssociatedTokenProgram')
    case HnsWallet.SOLANA_METADATA_PROGRAM_ID:
      return getLocale('hnsWalletSolanaMetaDataProgram')
    case HnsWallet.SOLANA_SYSVAR_RENT_PROGRAM_ID:
      return getLocale('hnsWalletSolanaSysvarRentProgram')
    case HnsWallet.SOLANA_TOKEN_PROGRAM_ID:
      return getLocale('hnsWalletSolanaTokenProgram')
    default:
      return programId
  }
}
