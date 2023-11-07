// Copyright (c) 2023 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

import { AccountPageTabs, HnsWallet, WalletRoutes } from '../constants/types'

export const makeAccountRoute = (
  accountInfo: Pick<HnsWallet.AccountInfo, 'address' | 'accountId'>,
  tab: AccountPageTabs
) => {
  const id = accountInfo.address || accountInfo.accountId.uniqueKey
  return WalletRoutes.Account.replace(':accountId', id).replace(
    ':selectedTab?',
    tab
  )
}

export const makeAccountTransactionRoute = (
  accountInfo: Pick<HnsWallet.AccountInfo, 'address' | 'accountId'>,
  transactionId: string
) => {
  const id = accountInfo.address || accountInfo.accountId.uniqueKey
  return (
    WalletRoutes.Account.replace(':accountId', id).replace(
      ':selectedTab?',
      AccountPageTabs.AccountTransactionsSub
    ) + '#' + transactionId.replace('#', '')
  )
}
