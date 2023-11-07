// Copyright (c) 2021 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at https://mozilla.org/MPL/2.0/.

import { HnsWallet } from '../../constants/types'

export type AccountPayloadType = {
  selectedAccounts: HnsWallet.AccountInfo[]
}

export type ConnectWithSitePayloadType = {
  addressToConnect: string,
  duration: HnsWallet.PermissionLifetimeOption
}

export type ShowConnectToSitePayload = {
  accounts: string[]
  originInfo: HnsWallet.OriginInfo
}

export type EthereumChainRequestPayload = {
  chainId: string
  approved: boolean
}

export type SignMessageHardwarePayload = {
  request: HnsWallet.SignMessageRequest
  account: HnsWallet.AccountInfo
}

export type SignMessageProcessedPayload = {
  approved: boolean
  id: number
  signature?: HnsWallet.ByteArrayStringUnion
  error?: string
}

export type SignAllTransactionsProcessedPayload = {
  approved: boolean
  id: number
  signatures?: HnsWallet.ByteArrayStringUnion[]
  error?: string
}

export type SwitchEthereumChainProcessedPayload = {
  requestId: string
  approved: boolean
}

export type GetEncryptionPublicKeyProcessedPayload = {
  requestId: string
  approved: boolean
}

export type DecryptProcessedPayload = {
  requestId: string
  approved: boolean
}

export type SignTransactionHardwarePayload = {
  request: HnsWallet.SignTransactionRequest
  account: HnsWallet.AccountInfo
}

export type SignAllTransactionsHardwarePayload = {
  request: HnsWallet.SignAllTransactionsRequest
  account: HnsWallet.AccountInfo
}
