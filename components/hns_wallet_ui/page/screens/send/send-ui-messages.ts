// Copyright (c) 2022 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

import { AddressMessageInfo } from '../../../constants/types'

export const ENSOffchainLookupMessage: AddressMessageInfo = {
  title: 'hnsWalletEnsOffChainLookupTitle',
  description: 'hnsWalletEnsOffChainLookupDescription',
  url: 'https://github.com/hnsvn/hns-browser/wiki/ENS-offchain-lookup'
}

export const FailedChecksumMessage: AddressMessageInfo = {
  title: 'hnsWalletFailedChecksumTitle',
  description: 'hnsWalletFailedChecksumDescription'
}

export const FEVMAddressConvertionMessage: AddressMessageInfo = {
  title: 'hnsWalletFEVMAddressTranslationTitle',
  description: 'hnsWalletFEVMAddressTranslationDescription',
  url: 'https://docs.filecoin.io/smart-contracts/filecoin-evm-runtime/address-types/',
  type: 'warning',
}