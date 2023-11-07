// Copyright (c) 2023 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.
import { useMemo } from 'react'
import { skipToken } from '@reduxjs/toolkit/query/react'

// hooks
import { useSafeWalletSelector } from './use-safe-selector'
import { useGetTokenBalancesRegistryQuery } from '../slices/api.slice'
import { querySubscriptionOptions60s } from '../slices/constants'

// Types / constants
import { HnsWallet, CoinTypes } from '../../constants/types'
import { WalletSelectors } from '../selectors'

// Utils
import { networkSupportsAccount } from '../../utils/network-utils'

interface Arg {
  networks: HnsWallet.NetworkInfo[]
  accounts: HnsWallet.AccountInfo[]
}

const coinTypesMapping = {
  [HnsWallet.CoinType.SOL]: CoinTypes.SOL,
  [HnsWallet.CoinType.ETH]: CoinTypes.ETH,
  [HnsWallet.CoinType.FIL]: CoinTypes.FIL,
  [HnsWallet.CoinType.BTC]: CoinTypes.BTC,
}

export const useBalancesFetcher = (arg: Arg | typeof skipToken) => {
  // redux
  const isWalletLocked = useSafeWalletSelector(WalletSelectors.isWalletLocked)
  const isWalletCreated = useSafeWalletSelector(WalletSelectors.isWalletCreated)
  const hasInitialized = useSafeWalletSelector(WalletSelectors.hasInitialized)

  const args = useMemo(() => arg !== skipToken && arg.accounts && arg.networks
    ? arg.accounts.flatMap(account =>
        arg.networks
        .filter(network => networkSupportsAccount(network, account.accountId))
        .map(network => ({
          accountId: account.accountId,
          chainId: network.chainId,
          coin: coinTypesMapping[network.coin]
        }))
        .filter(({ coin }) => coin !== undefined)
      )
    : skipToken,
    [arg]
  )

  return useGetTokenBalancesRegistryQuery(
    args !== skipToken &&
    !isWalletLocked &&
    isWalletCreated &&
    hasInitialized
      ? args
      : skipToken,
    querySubscriptionOptions60s
  )
}

export default useBalancesFetcher
