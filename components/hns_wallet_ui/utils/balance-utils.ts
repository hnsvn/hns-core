// Copyright (c) 2022 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at https://mozilla.org/MPL/2.0/.

// types
import { TokenBalancesRegistry } from '../common/slices/entities/token-balance.entity'
import { HnsWallet } from '../constants/types'

// utils
import Amount from './amount'

export const formatTokenBalanceWithSymbol = (balance: string, decimals: number, symbol: string, decimalPlace?: number) => {
  return `${new Amount(balance)
    .divideByDecimals(decimals)
    .format(decimalPlace ?? 6, true)} ${symbol}`
}

export const getAccountBalancesKey = (
  accountId: Pick<HnsWallet.AccountId, 'uniqueKey'>
) => {
  return accountId.uniqueKey
}

export const getPercentAmount = (
  asset: HnsWallet.BlockchainToken,
  accountId: HnsWallet.AccountId,
  percent: number,
  tokenBalancesRegistry: TokenBalancesRegistry | undefined
): string => {
  const assetBalance = getBalance(accountId, asset, tokenBalancesRegistry) || '0'
  const amountWrapped = new Amount(assetBalance).times(percent)

  const formattedAmount =
    percent === 1
      ? amountWrapped.divideByDecimals(asset.decimals).format()
      : amountWrapped.divideByDecimals(asset.decimals).format(6)

  return formattedAmount
}

export const getBalance = (
  accountId: HnsWallet.AccountId | undefined,
  asset: HnsWallet.BlockchainToken | undefined,
  tokenBalancesRegistry: TokenBalancesRegistry | undefined
) => {
  if (!accountId || !asset || !tokenBalancesRegistry) {
    return ''
  }

  const accountBalances =
    tokenBalancesRegistry[getAccountBalancesKey(accountId)]
  if (!accountBalances) {
    return '0'
  }

  const chainIdBalances =
    accountBalances[asset.chainId] ??
    accountBalances[asset.chainId.toString()]
  if (!chainIdBalances) {
    return '0'
  }

  const balance =
    chainIdBalances[asset.contractAddress] ??
    chainIdBalances[asset.contractAddress.toLowerCase()]
  return balance || '0'
}
