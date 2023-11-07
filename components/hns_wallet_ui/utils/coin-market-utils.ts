// Copyright (c) 2022 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at https://mozilla.org/MPL/2.0/.

import Fuse from 'fuse.js'
import { HnsWallet, MarketAssetFilterOption, MarketGridColumnTypes, SortOrder } from '../constants/types'

const searchOptions: Fuse.IFuseOptions<HnsWallet.CoinMarket> = {
  shouldSort: true,
  threshold: 0.1,
  location: 0,
  distance: 0,
  minMatchCharLength: 1,
  keys: [
    { name: 'name', weight: 0.5 },
    { name: 'symbol', weight: 0.5 }
  ]
}

export const sortCoinMarkets = (marketData: HnsWallet.CoinMarket[], sortOrder: SortOrder, columnId: MarketGridColumnTypes) => {
  if (sortOrder === 'asc') {
    return marketData.sort((a, b) => a[columnId] - b[columnId])
  } else {
    return marketData.sort((a, b) => b[columnId] - a[columnId])
  }
}

export const searchCoinMarkets = (searchList: HnsWallet.CoinMarket[], searchTerm: string): HnsWallet.CoinMarket[] => {
  if (!searchTerm) {
    return searchList
  }

  const fuse = new Fuse(searchList, searchOptions)
  return fuse.search(searchTerm)
    .map((result: Fuse.FuseResult<HnsWallet.CoinMarket>) => result.item)
}

export const filterCoinMarkets = (coins: HnsWallet.CoinMarket[], tradableAssets: HnsWallet.BlockchainToken[], filter: MarketAssetFilterOption) => {
  const tradableAssetsSymbols = tradableAssets.map(asset => asset.symbol.toLowerCase())

  if (filter === 'all') {
    return coins
  } else if (filter === 'tradable') {
    return coins.filter(asset => tradableAssetsSymbols.includes(asset.symbol.toLowerCase()))
  }

  return []
}
