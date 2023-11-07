// Copyright (c) 2022 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at https://mozilla.org/MPL/2.0/.
import { HnsWallet, CreateAccountOptionsType } from '../constants/types'
import { getLocale } from '../../common/locale'
import { getNetworkLogo } from './asset-options'

export const CreateAccountOptions = (options: {
  isFilecoinEnabled: boolean
  isSolanaEnabled: boolean
  isBitcoinEnabled: boolean
}): CreateAccountOptionsType[] => {
  let accounts = [
    {
      description: getLocale('hnsWalletCreateAccountEthereumDescription'),
      name: 'Ethereum',
      coin: HnsWallet.CoinType.ETH,
      icon: getNetworkLogo(HnsWallet.MAINNET_CHAIN_ID, 'ETH')
    }
  ]
  if (options.isSolanaEnabled) {
    accounts.push({
      description: getLocale('hnsWalletCreateAccountSolanaDescription'),
      name: 'Solana',
      coin: HnsWallet.CoinType.SOL,
      icon: getNetworkLogo(HnsWallet.SOLANA_MAINNET, 'SOL')
    })
  }
  if (options.isFilecoinEnabled) {
    accounts.push({
      description: getLocale('hnsWalletCreateAccountFilecoinDescription'),
      name: 'Filecoin',
      coin: HnsWallet.CoinType.FIL,
      icon: getNetworkLogo(HnsWallet.FILECOIN_MAINNET, 'FIL')
    })
  }
  if (options.isBitcoinEnabled) {
    accounts.push({
      description: getLocale('hnsWalletCreateAccountBitcoinDescription'),
      name: 'Bitcoin',
      coin: HnsWallet.CoinType.BTC,
      icon: getNetworkLogo(HnsWallet.BITCOIN_MAINNET, 'BTC')
    })
  }
  return accounts
}
