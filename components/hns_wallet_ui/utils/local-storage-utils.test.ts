// Copyright (c) 2023 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

import {
  HnsWallet
} from '../constants/types'
import {
  parseJSONFromLocalStorage,
  makeInitialFilteredOutNetworkKeys
} from './local-storage-utils'
import {
  networkEntityAdapter
} from '../common/slices/entities/network.entity'
import { AllNetworksOptionDefault } from '../options/network-filter-options'

const mockSolanaOption = {
  chainId: '0x65',
  coin: 501
}

const mockInitialFilteredOutNetworkKeys = [
  networkEntityAdapter.selectId(
    {
      chainId: HnsWallet.GOERLI_CHAIN_ID,
      coin: HnsWallet.CoinType.ETH
    }
  ).toString(),
  networkEntityAdapter.selectId(
    {
      chainId: HnsWallet.SEPOLIA_CHAIN_ID,
      coin: HnsWallet.CoinType.ETH
    }
  ).toString(),
  networkEntityAdapter.selectId(
    {
      chainId: HnsWallet.SOLANA_DEVNET,
      coin: HnsWallet.CoinType.SOL
    }
  ).toString(),
  networkEntityAdapter.selectId(
    {
      chainId: HnsWallet.SOLANA_TESTNET,
      coin: HnsWallet.CoinType.SOL
    }
  ).toString(),
  networkEntityAdapter.selectId(
    {
      chainId: HnsWallet.FILECOIN_TESTNET,
      coin: HnsWallet.CoinType.FIL
    }
  ).toString(),
  networkEntityAdapter.selectId(
    {
      chainId: HnsWallet.FILECOIN_ETHEREUM_TESTNET_CHAIN_ID,
      coin: HnsWallet.CoinType.ETH
    }
  ).toString(),
  networkEntityAdapter.selectId(
    {
      chainId: HnsWallet.BITCOIN_TESTNET,
      coin: HnsWallet.CoinType.BTC
    }
  ).toString(),
  networkEntityAdapter.selectId(
    {
      chainId: HnsWallet.LOCALHOST_CHAIN_ID,
      coin: HnsWallet.CoinType.SOL
    }
  ).toString(),
  networkEntityAdapter.selectId(
    {
      chainId: HnsWallet.LOCALHOST_CHAIN_ID,
      coin: HnsWallet.CoinType.ETH
    }
  ).toString(),
  networkEntityAdapter.selectId(
    {
      chainId: HnsWallet.LOCALHOST_CHAIN_ID,
      coin: HnsWallet.CoinType.FIL
    }
  ).toString(),
  networkEntityAdapter.selectId(
    {
      chainId: HnsWallet.LOCALHOST_CHAIN_ID,
      coin: HnsWallet.CoinType.BTC
    }
  ).toString()
]

describe('Test parseJSONFromLocalStorage', () => {
  const key = 'PORTFOLIO_NETWORK_FILTER_OPTION'
  const mockValue = JSON.stringify(mockSolanaOption)
  let mockLocalStorageGet = jest.fn()
  Object.defineProperty(window, 'localStorage', {
    value: {
      getItem: mockLocalStorageGet
    }
  })
  it('getItem should return a value and parse correctly to return mockSolanaOption.', () => {
    mockLocalStorageGet.mockReturnValue(mockValue)
    expect(parseJSONFromLocalStorage(key, AllNetworksOptionDefault)).toEqual(mockSolanaOption)
    expect(window.localStorage.getItem).toHaveBeenCalledWith(key)
    expect(jest.isMockFunction(window.localStorage.getItem)).toBe(true)
    expect(mockLocalStorageGet.mock.results[0].value).toBe(mockValue)
  })
  it('getItem should return null, then parse should fail and return the fallback AllNetworksOptionDefault.', () => {
    mockLocalStorageGet.mockReturnValue(null)
    expect(parseJSONFromLocalStorage(key, AllNetworksOptionDefault)).toEqual(AllNetworksOptionDefault)
    expect(window.localStorage.getItem).toHaveBeenCalledWith(key)
    expect(jest.isMockFunction(window.localStorage.getItem)).toBe(true)
    expect(mockLocalStorageGet.mock.results[0].value).toBe(null)
  })
})

describe('Test makeInitialFilteredOutNetworkKeys', () => {
  it('Should construct a string array of test network keys', () => {
    expect(makeInitialFilteredOutNetworkKeys())
      .toEqual(mockInitialFilteredOutNetworkKeys)
  })
})
