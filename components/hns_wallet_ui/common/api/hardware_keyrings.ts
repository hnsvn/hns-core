/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

import { assert } from 'chrome://resources/js/assert_ts.js'
import EthereumLedgerBridgeKeyring from '../../common/hardware/ledgerjs/eth_ledger_bridge_keyring'
import SolanaLedgerBridgeKeyring from '../../common/hardware/ledgerjs/sol_ledger_bridge_keyring'
import TrezorBridgeKeyring from '../../common/hardware/trezor/trezor_bridge_keyring'
import { HnsWallet } from '../../constants/types'
import * as HWInterfaces from '../hardware/interfaces'
import FilecoinLedgerBridgeKeyring from '../../common/hardware/ledgerjs/fil_ledger_bridge_keyring'

export type HardwareKeyring = HWInterfaces.LedgerEthereumKeyring | HWInterfaces.TrezorKeyring | HWInterfaces.LedgerFilecoinKeyring | HWInterfaces.LedgerSolanaKeyring

export function getCoinName (coin: HnsWallet.CoinType) {
  switch (coin) {
    case HnsWallet.CoinType.FIL:
      return 'Filecoin'
    case HnsWallet.CoinType.ETH:
      return 'Ethereum'
  }
  return ''
}

const VendorTypes = [
  HnsWallet.TREZOR_HARDWARE_VENDOR,
  HnsWallet.LEDGER_HARDWARE_VENDOR
] as const
export type HardwareVendor = typeof VendorTypes[number]

// Lazy instances for keyrings
let ethereumHardwareKeyring: EthereumLedgerBridgeKeyring
let filecoinHardwareKeyring: FilecoinLedgerBridgeKeyring
let solanaHardwareKeyring: SolanaLedgerBridgeKeyring
let trezorHardwareKeyring: TrezorBridgeKeyring

export function getHardwareKeyring (
  type: HardwareVendor,
  coin: HnsWallet.CoinType = HnsWallet.CoinType.ETH,
  onAuthorized?: () => void
): EthereumLedgerBridgeKeyring | HWInterfaces.TrezorKeyring | FilecoinLedgerBridgeKeyring | SolanaLedgerBridgeKeyring {
  if (type === HnsWallet.LEDGER_HARDWARE_VENDOR) {
    if (coin === HnsWallet.CoinType.ETH) {
      return getLedgerEthereumHardwareKeyring(onAuthorized)
    } else if (coin === HnsWallet.CoinType.FIL) {
      return getLedgerFilecoinHardwareKeyring(onAuthorized)
    } else if (coin === HnsWallet.CoinType.SOL) {
      return getLedgerSolanaHardwareKeyring(onAuthorized)
    }
  }

  const trezorKeyring = getTrezorHardwareKeyring()
  assert(type === trezorHardwareKeyring.type(), '')
  return trezorKeyring
}

export function getLedgerEthereumHardwareKeyring (onAuthorized?: () => void): EthereumLedgerBridgeKeyring {
  if (!ethereumHardwareKeyring) {
    ethereumHardwareKeyring = new EthereumLedgerBridgeKeyring(onAuthorized)
  }
  return ethereumHardwareKeyring
}

export function getLedgerFilecoinHardwareKeyring (onAuthorized?: () => void): FilecoinLedgerBridgeKeyring {
  if (!filecoinHardwareKeyring) {
    filecoinHardwareKeyring = new FilecoinLedgerBridgeKeyring(onAuthorized)
  }
  return filecoinHardwareKeyring
}

export function getLedgerSolanaHardwareKeyring (onAuthorized?: () => void): SolanaLedgerBridgeKeyring {
  if (!solanaHardwareKeyring) {
    solanaHardwareKeyring = new SolanaLedgerBridgeKeyring(onAuthorized)
  }
  return solanaHardwareKeyring
}

export function getTrezorHardwareKeyring (): TrezorBridgeKeyring {
  if (!trezorHardwareKeyring) {
    trezorHardwareKeyring = new TrezorBridgeKeyring()
  }
  return trezorHardwareKeyring
}
