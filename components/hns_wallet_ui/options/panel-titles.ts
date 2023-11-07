// Copyright (c) 2021 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at https://mozilla.org/MPL/2.0/.
import { PanelTitleObjectType } from '../constants/types'
import { getLocale } from '../../common/locale'

export const PanelTitles = (): PanelTitleObjectType[] => [
  {
    id: 'buy',
    title: getLocale('hnsWalletBuy')
  },
  {
    id: 'send',
    title: getLocale('hnsWalletSend')
  },
  {
    id: 'swap',
    title: getLocale('hnsWalletSwap')
  },
  {
    id: 'apps',
    title: getLocale('hnsWalletTopTabApps')
  },
  {
    id: 'sitePermissions',
    title: getLocale('hnsWalletSitePermissionsTitle')
  },
  {
    id: 'activity', // Transactions
    title: getLocale('hnsWalletActivity')
  },
  {
    id: 'transactionDetails',
    title: getLocale('hnsWalletTransactionDetails')
  },
  {
    id: 'assets',
    title: getLocale('hnsWalletAssetsPanelTitle')
  },
  {
    id: 'currencies',
    title: getLocale('hnsWalletSelectCurrency')
  }
]
