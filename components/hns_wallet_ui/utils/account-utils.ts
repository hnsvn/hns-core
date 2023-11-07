// Copyright (c) 2022 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at https://mozilla.org/MPL/2.0/.

import { assertNotReached } from 'chrome://resources/js/assert_ts.js';
import { getLocale } from '../../common/locale'

// types
import {
  HnsWallet,
  WalletAccountTypeName
} from '../constants/types'

// constants
import registry from '../common/constants/registry'

// utils
import { reduceAddress } from './reduce-address'
import { EntityState } from '@reduxjs/toolkit'

export const sortAccountsByName = (accounts: HnsWallet.AccountInfo[]) => {
  return [...accounts].sort(function (
    a: HnsWallet.AccountInfo,
    b: HnsWallet.AccountInfo
  ) {
    if (a.name < b.name) {
      return -1
    }

    if (a.name > b.name) {
      return 1
    }

    return 0
  })
}

export const groupAccountsById = (
  accounts: HnsWallet.AccountInfo[],
  key: string
) => {
  return accounts.reduce<Record<string, HnsWallet.AccountInfo[]>>(
    (result, obj) => {
      ;(result[obj[key]] = result[obj[key]] || []).push(obj)
      return result
    },
    {}
  )
}

export const findAccountByUniqueKey = <
  T extends { accountId: { uniqueKey: string } }
>(
  accounts: T[],
  uniqueKey: string | undefined
): T | undefined => {
  if (!uniqueKey) {
    return
  }

  return accounts.find((account) => uniqueKey === account.accountId.uniqueKey)
}

export const getAccountType = (
  info: Pick<HnsWallet.AccountInfo, 'accountId' | 'hardware'>
): WalletAccountTypeName => {
  if (info.accountId.kind === HnsWallet.AccountKind.kHardware) {
    return info.hardware!.vendor as 'Ledger' | 'Trezor'
  }
  return info.accountId.kind === HnsWallet.AccountKind.kImported
    ? 'Secondary'
    : 'Primary'
}

export const entityIdFromAccountId = (
  accountId: Pick<HnsWallet.AccountId, 'address' | 'uniqueKey'>
) => {
  // TODO(apaymyshev): should use uniqueKey always
  return accountId.address || accountId.uniqueKey
}

export const findAccountByAddress = (
  address: string,
  accounts: EntityState<HnsWallet.AccountInfo> | undefined
): HnsWallet.AccountInfo | undefined => {
  if (!address || ! accounts)
    return undefined
  for (const id of accounts.ids) {
    if (accounts.entities[id]?.address.toLowerCase() === address.toLowerCase()) {
      return accounts.entities[id]
    }
  }
  return undefined
}

export const findAccountByAccountId = (
  accountId: HnsWallet.AccountId,
  accounts: EntityState<HnsWallet.AccountInfo> | undefined
): HnsWallet.AccountInfo | undefined => {
  if (!accounts) {
    return undefined
  }
  return accounts.entities[entityIdFromAccountId(accountId)]
}

export const getAddressLabel = (
  address: string,
  accounts: EntityState<HnsWallet.AccountInfo>
): string => {
  return (
    registry[address.toLowerCase()] ??
    findAccountByAddress(address, accounts)?.name ??
    reduceAddress(address)
  )
}

export const getAccountLabel = (
  accountId: HnsWallet.AccountId,
  accounts: EntityState<HnsWallet.AccountInfo>
): string => {
  return (
    findAccountByAccountId(accountId, accounts)?.name ??
    reduceAddress(accountId.address)
  )
}

export function isHardwareAccount(account: HnsWallet.AccountId) {
  return account.kind === HnsWallet.AccountKind.kHardware
}

export const keyringIdForNewAccount = (
  coin: HnsWallet.CoinType,
  chainId?: string | undefined
) => {
  if (coin === HnsWallet.CoinType.ETH) {
    return HnsWallet.KeyringId.kDefault
  }

  if (coin === HnsWallet.CoinType.SOL) {
    return HnsWallet.KeyringId.kSolana
  }

  if (coin === HnsWallet.CoinType.FIL) {
    if (chainId === HnsWallet.FILECOIN_MAINNET) {
      return HnsWallet.KeyringId.kFilecoin
    }
    if (chainId === HnsWallet.FILECOIN_TESTNET) {
      return HnsWallet.KeyringId.kFilecoinTestnet
    }
  }

  if (coin === HnsWallet.CoinType.BTC) {
    if (chainId === HnsWallet.BITCOIN_MAINNET) {
      return HnsWallet.KeyringId.kBitcoin84
    }
    if (chainId === HnsWallet.BITCOIN_TESTNET) {
      return HnsWallet.KeyringId.kBitcoin84Testnet
    }
  }

  assertNotReached(`Unknown coin ${coin} and chainId ${chainId}`)
}

export const getAccountTypeDescription = (coin: HnsWallet.CoinType) => {
  switch (coin) {
    case HnsWallet.CoinType.ETH:
      return getLocale('hnsWalletETHAccountDescrption')
    case HnsWallet.CoinType.SOL:
      return getLocale('hnsWalletSOLAccountDescrption')
    case HnsWallet.CoinType.FIL:
      return getLocale('hnsWalletFILAccountDescrption')
    case HnsWallet.CoinType.BTC:
      return getLocale('hnsWalletBTCAccountDescrption')
    default:
      assertNotReached(`Unknown coin ${coin}`)
  }
}
