// Copyright (c) 2022 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at https://mozilla.org/MPL/2.0/.

// types
import { HnsWallet } from '../../constants/types'

/**
 * There are 1-billion lamports in one SOL
 */
export const LAMPORTS_PER_SOL = 1000000000

export const SolanaTransactionTypes = [
  HnsWallet.TransactionType.SolanaSystemTransfer,
  HnsWallet.TransactionType.SolanaSPLTokenTransfer,
  HnsWallet.TransactionType.SolanaSPLTokenTransferWithAssociatedTokenAccountCreation,
  HnsWallet.TransactionType.SolanaDappSignTransaction,
  HnsWallet.TransactionType.SolanaDappSignAndSendTransaction,
  HnsWallet.TransactionType.SolanaSwap
]
