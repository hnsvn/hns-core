// Copyright (c) 2022 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at https://mozilla.org/MPL/2.0/.

// types
import {
  HnsWallet,
  SerializableSolanaTxData,
  WalletAccountTypeName
} from '../constants/types'

export const emptyNetwork: HnsWallet.NetworkInfo = {
  chainId: '',
  chainName: '',
  activeRpcEndpointIndex: 0,
  rpcEndpoints: [],
  blockExplorerUrls: [],
  iconUrls: [],
  symbol: '',
  symbolName: '',
  decimals: 0,
  coin: HnsWallet.CoinType.ETH,
  supportedKeyrings: [],
  isEip1559: true
}

export const getNetworkInfo = (chainId: string, coin: HnsWallet.CoinType, list: HnsWallet.NetworkInfo[]) => {
  for (let it of list) {
    if (it.chainId === chainId && it.coin === coin) {
      return it
    }
  }
  return emptyNetwork
}

export const networkSupportsAccount = (
  network: HnsWallet.NetworkInfo,
  accountId: HnsWallet.AccountId
) => {
  return (
    network.coin === accountId.coin &&
    network.supportedKeyrings.includes(accountId.keyringId)
  )
}

export const filterNetworksForAccount = (
  networks: HnsWallet.NetworkInfo[],
  accountId: HnsWallet.AccountId
): HnsWallet.NetworkInfo[] => {
  if (!networks) {
    return []
  }
  return networks.filter((network) => networkSupportsAccount(network, accountId))
}

export const getTokensNetwork = (networks: HnsWallet.NetworkInfo[], token: HnsWallet.BlockchainToken): HnsWallet.NetworkInfo => {
  if (!networks) {
    return emptyNetwork
  }

  const network = networks.filter((n) => n.chainId === token.chainId)
  if (network.length > 1) {
    return network?.find((n) => n.symbol.toLowerCase() === token.symbol.toLowerCase()) ?? emptyNetwork
  }

  return network[0] ?? emptyNetwork
}

export type TxDataPresence = {
  ethTxData?: Partial<HnsWallet.TxDataUnion['ethTxData']> | undefined
  ethTxData1559?: Partial<HnsWallet.TxDataUnion['ethTxData1559']> | undefined
  solanaTxData?: Partial<HnsWallet.TxDataUnion['solanaTxData']>
    | SerializableSolanaTxData
    | undefined
  filTxData?: Partial<HnsWallet.TxDataUnion['filTxData']> | undefined
}

export const getCoinFromTxDataUnion = <T extends TxDataPresence> (txDataUnion: T): HnsWallet.CoinType => {
  if (txDataUnion.filTxData) { return HnsWallet.CoinType.FIL }
  if (txDataUnion.solanaTxData) { return HnsWallet.CoinType.SOL }
  // TODO(apaymyshev): bitcoin support
  return HnsWallet.CoinType.ETH
}

const EIP1559_SUPPORTED_ACCOUNT_TYPE_NAMES = [
  'Primary',
  'Secondary',
  'Ledger',
  'Trezor'
]

/**
 * Check if the keyring associated with the given account AND the network
 * support the EIP-1559 fee market for paying gas fees.
 *
 * This method can also be used to determine if the given parameters support
 * EVM Type-2 transactions. The return value is always false for non-EVM
 * networks.
 *
 * @param {HnsWallet.AccountInfo} account
 * @param {HnsWallet.NetworkInfo} network
 * @returns {boolean} Returns a boolean result indicating EIP-1559 support.
 */
export const hasEIP1559Support = (
  accountType: WalletAccountTypeName,
  network: HnsWallet.NetworkInfo
) => {
  return (
    EIP1559_SUPPORTED_ACCOUNT_TYPE_NAMES.includes(accountType) &&
    network.isEip1559
  )
}

export const reduceNetworkDisplayName = (name?: string) => {
  if (!name) {
    return ''
  }
  return name.split(' ')[0]
}
