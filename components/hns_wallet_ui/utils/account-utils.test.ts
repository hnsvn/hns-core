// Copyright (c) 2023 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

import { HnsWallet } from '../constants/types'

import { mockAccount } from '../common/constants/mocks'

import {
  isHardwareAccount,
  findAccountByAddress,
  getAccountTypeDescription
} from './account-utils'

import {
  AccountInfoEntity,
  accountInfoEntityAdaptor
} from '../common/slices/entities/account-info.entity'

const mockHardware = {
  deviceId: 'testDeviceId',
  path: '',
  vendor: 'Ledger'
}

const mockHardwareAccounts: HnsWallet.AccountInfo[] = [
  {
    ...mockAccount,
    accountId: {
      ...mockAccount.accountId,
      kind: HnsWallet.AccountKind.kHardware
    },
    hardware: mockHardware
  },
  {
    ...mockAccount,
    accountId: {
      ...mockAccount.accountId,
      kind: HnsWallet.AccountKind.kHardware,
      address: 'mockAccount2'
    },
    address: 'mockAccount2',
    hardware: {
      ...mockHardware,
      deviceId: 'testDeviceId2'
    }
  }
]

const mockAccounts: AccountInfoEntity[] = [
  {
    ...mockAccount,
  },
  {
    ...mockAccount,
    accountId: {
      ...mockAccount.accountId,
      address: 'mockAccount2'
    },
    address: 'mockAccount2',
  }
]

const mockAccountsRegistry = accountInfoEntityAdaptor.setAll(
  accountInfoEntityAdaptor.getInitialState(),
  mockAccounts
)

describe('Account Utils', () => {
  describe('isHardwareAccount', () => {
    it.each(mockHardwareAccounts)(
      'should return true if accounts have deviceId and address matches',
      (account) => {
        expect(isHardwareAccount(account.accountId)).toBe(true)
      }
    )
  })
  describe('findAccountFromRegistry', () => {
    it.each(mockAccounts)(
      'should return true if accounts have deviceId and address matches',
      (account) => {
        expect(
          findAccountByAddress(account.address, mockAccountsRegistry)
        ).toBe(account)
      }
    )
  })
})

describe('Test getAccountTypeDescription', () => {
  test('CoinType ETH Address Description', () => {
    expect(getAccountTypeDescription(HnsWallet.CoinType.ETH))
      .toEqual('hnsWalletETHAccountDescrption')
  })
  test('CoinType SOL Address Description', () => {
    expect(getAccountTypeDescription(HnsWallet.CoinType.SOL))
      .toEqual('hnsWalletSOLAccountDescrption')
  })
  test('CoinType FIL Address Description', () => {
    expect(getAccountTypeDescription(HnsWallet.CoinType.FIL))
      .toEqual('hnsWalletFILAccountDescrption')
  })
  test('CoinType BTC Address Description', () => {
    expect(getAccountTypeDescription(HnsWallet.CoinType.BTC))
      .toEqual('hnsWalletBTCAccountDescrption')
  })
})
