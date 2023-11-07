// Copyright (c) 2022 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at https://mozilla.org/MPL/2.0/.

import { getLocale } from '$web-common/locale'
import { MarketGridHeader } from '../constants/types'

// The id field matches values in MarketGridColumnTypes
// which match property fields in CoinMarketMetadata
// this helps in finding the correct header to sort by
export const marketGridHeaders: MarketGridHeader[] = [
  {
    id: 'assets',
    label: getLocale('hnsWalletMarketDataAssetsColumn'),
  },
  {
    id: 'currentPrice',
    label: getLocale('hnsWalletMarketDataPriceColumn'),
    sortable: true,
  },
  {
    id: 'priceChangePercentage24h',
    label: getLocale('hnsWalletMarketData24HrColumn'),
    sortable: true,
  },
  {
    id: 'marketCap',
    label: getLocale('hnsWalletMarketDataMarketCapColumn'),
    sortable: true,
    width: '90px',
    hideOnPanel: true
  },
  {
    id: 'totalVolume',
    label: getLocale('hnsWalletMarketDataVolumeColumn'),
    sortable: true,
    hideOnPanel: true,
    width: '95px'
  },
  {
    id: 'actions',
    label: 'Buy/Deposit',
    sortable: false,
    hideOnPanel: true,
    width: '130px'
  }
]
