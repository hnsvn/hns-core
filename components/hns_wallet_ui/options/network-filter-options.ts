// Copyright (c) 2022 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at https://mozilla.org/MPL/2.0/.
import { HnsWallet, NetworkFilterType } from '../constants/types'
import { getLocale } from '../../common/locale'
import AllNetworksIcon from '../assets/svg-icons/all-networks-icon.svg'

export const AllNetworksOption: HnsWallet.NetworkInfo = {
  blockExplorerUrls: [],
  chainId: 'all',
  chainName: getLocale('hnsWalletNetworkFilterAll'),
  coin: 0,
  decimals: 0,
  iconUrls: [AllNetworksIcon],
  activeRpcEndpointIndex: 0,
  rpcEndpoints: [],
  symbol: 'all',
  symbolName: 'all',
  supportedKeyrings: [],
  isEip1559: false
}

export const AllNetworksOptionDefault: NetworkFilterType = {
  chainId: 'all',
  coin: 0
}

export const SupportedTopLevelChainIds = [
  HnsWallet.MAINNET_CHAIN_ID,
  HnsWallet.SOLANA_MAINNET,
  HnsWallet.FILECOIN_MAINNET
]
