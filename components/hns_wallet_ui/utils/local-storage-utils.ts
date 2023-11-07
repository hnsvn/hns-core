// Copyright (c) 2023 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

import {
  SupportedTestNetworks,
  SupportedCoinTypes,
  HnsWallet
} from '../constants/types'
import {
  networkEntityAdapter
} from '../common/slices/entities/network.entity'
import { LOCAL_STORAGE_KEYS } from '../common/constants/local-storage-keys'

export const parseJSONFromLocalStorage = <T = any> (
  storageString: keyof typeof LOCAL_STORAGE_KEYS,
  fallback: T
): T => {
  try {
    return JSON.parse(
      window.localStorage.getItem(LOCAL_STORAGE_KEYS[storageString]) || ''
    ) as T
  } catch (e) {
    return fallback
  }
}

export const makeInitialFilteredOutNetworkKeys = () => {
  const localHostNetworkKeys = SupportedCoinTypes.map((coin) => {
    return networkEntityAdapter.selectId(
      {
        chainId: HnsWallet.LOCALHOST_CHAIN_ID,
        coin: coin
      }
    ).toString()
  })
  const testNetworkKeys = SupportedTestNetworks
    .filter((chainId) => chainId !== HnsWallet.LOCALHOST_CHAIN_ID)
    .map((chainId) => {
      if (
        chainId === HnsWallet.SOLANA_DEVNET ||
        chainId === HnsWallet.SOLANA_TESTNET
      ) {
        return networkEntityAdapter.selectId(
          {
            chainId: chainId,
            coin: HnsWallet.CoinType.SOL
          }
        ).toString()
      }
      if (chainId === HnsWallet.FILECOIN_TESTNET) {
        return networkEntityAdapter.selectId(
          {
            chainId: chainId,
            coin: HnsWallet.CoinType.FIL
          }
        ).toString()
      }
      return networkEntityAdapter.selectId(
        {
          chainId: chainId,
          coin: HnsWallet.CoinType.ETH
        }
      ).toString()
    })
  return [...testNetworkKeys, ...localHostNetworkKeys]
}
