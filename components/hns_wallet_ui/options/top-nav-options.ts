// Copyright (c) 2022 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at https://mozilla.org/MPL/2.0/.

import { TopTabNavObjectType } from '../constants/types'
import { getLocale } from '$web-common/locale'

export const TopNavOptions = (): TopTabNavObjectType[] => [
  {
    id: 'portfolio',
    name: getLocale('hnsWalletTopNavPortfolio')
  },
  {
    id: 'nfts',
    name: getLocale('hnsWalletTopNavNFTS')
  },
  {
    id: 'activity', // Transactions
    name: getLocale('hnsWalletActivity')
  },
  {
    id: 'accounts',
    name: getLocale('hnsWalletTopNavAccounts')
  },
  {
    id: 'market',
    name: getLocale('hnsWalletTopNavMarket')
  }
  // Temp commented out for MVP
  // {
  //   id: 'apps',
  //   name: getLocale('hnsWalletTopTabApps')
  // }
]

export const TOP_NAV_OPTIONS = TopNavOptions()
