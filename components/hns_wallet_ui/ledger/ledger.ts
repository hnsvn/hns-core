/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

import { HnsWallet } from '../constants/types'
import { LedgerCommand, LEDGER_BRIDGE_URL } from '../common/hardware/ledgerjs/ledger-messages'
import { LedgerUntrustedMessagingTransport } from '../common/hardware/ledgerjs/ledger-untrusted-transport'
import { SolanaLedgerUntrustedMessagingTransport } from '../common/hardware/ledgerjs/sol-ledger-untrusted-transport'
import { EthereumLedgerUntrustedMessagingTransport } from '../common/hardware/ledgerjs/eth-ledger-untrusted-transport'
import { FilecoinLedgerUntrustedMessagingTransport } from '../common/hardware/ledgerjs/fil-ledger-untrusted-transport'

const setUpAuthorizeButtonListener = (targetUrl: string, coinType: HnsWallet.CoinType) => {
  const untrustedMessagingTransport = getUntrustedMessagingTransport(coinType, targetUrl)
  window.addEventListener('DOMContentLoaded', (event) => {
    const authorizeBtn = document.getElementById('authorize')
    if (authorizeBtn) {
      authorizeBtn.addEventListener('click', () => {
        untrustedMessagingTransport.promptAuthorization().then((result) => {
          untrustedMessagingTransport.sendCommand({
            id: LedgerCommand.AuthorizationSuccess,
            origin: LEDGER_BRIDGE_URL,
            command: LedgerCommand.AuthorizationSuccess
          })
        })
      })
    }
  })
}

const getUntrustedMessagingTransport = (coinType: HnsWallet.CoinType, targetUrl: string): LedgerUntrustedMessagingTransport => {
  switch (coinType) {
    case HnsWallet.CoinType.SOL:
      return new SolanaLedgerUntrustedMessagingTransport(window.parent, targetUrl)
    case HnsWallet.CoinType.ETH:
      return new EthereumLedgerUntrustedMessagingTransport(window.parent, targetUrl)
    case HnsWallet.CoinType.FIL:
      return new FilecoinLedgerUntrustedMessagingTransport(window.parent, targetUrl)
    default:
      throw new Error('Invalid coinType.')
  }
}

const params = new URLSearchParams(window.location.search)
const targetUrl = params.get('targetUrl')
const coinType = Number(params.get('coinType'))
if (targetUrl && coinType) {
  setUpAuthorizeButtonListener(targetUrl, coinType)
}
