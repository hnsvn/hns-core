// Copyright (c) 2021 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at https://mozilla.org/MPL/2.0/.

import {
  HnsWallet,
  FilecoinNetwork
} from '../../constants/types'

export type CreateWalletPayloadType = {
  password: string
}

export type AddAccountPayloadType = {
  accountName: string
  keyringId: HnsWallet.KeyringId
  coin: HnsWallet.CoinType
}

export type ImportAccountPayloadType = {
  accountName: string
  privateKey: string
  coin: HnsWallet.CoinType
  network?: FilecoinNetwork
}

export type ImportAccountFromJsonPayloadType = {
  accountName: string
  password: string
  json: string
}

export type RemoveAccountPayloadType = {
  accountId: HnsWallet.AccountId
  password: string
}

export type RestoreWalletPayloadType = {
  mnemonic: string
  password: string
  isLegacy: boolean
  completeWalletSetup?: boolean
}

export type WalletCreatedPayloadType = {
  mnemonic: string
}

export type ShowRecoveryPhrasePayload = {
  show: false
  password?: string
} | {
  show: true
  password: string
}

export type RecoveryWordsAvailablePayloadType = {
  mnemonic: string
}

export type UpdateSelectedAssetType = {
  asset?: HnsWallet.BlockchainToken | undefined
  timeFrame: HnsWallet.AssetPriceTimeframe
}

export type ImportFromExternalWalletPayloadType = {
  password: string
  newPassword: string
}

export type ImportWalletErrorPayloadType = {
  hasError: boolean
  errorMessage?: string
  incrementAttempts?: boolean
}

export type PinningStatusType = {
  code: HnsWallet.TokenPinStatusCode | undefined
  error: HnsWallet.PinError | undefined
}

export type NftsPinningStatusType = {
  [key: string]: PinningStatusType
}

export type UpdateNftPinningStatusType = {
  token: HnsWallet.BlockchainToken
  status?: HnsWallet.TokenPinStatus | undefined
  error?: HnsWallet.PinError | undefined
}
