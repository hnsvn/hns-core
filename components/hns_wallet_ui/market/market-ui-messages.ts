// Copyright (c) 2022 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at https://mozilla.org/MPL/2.0/.
// Copyright (c) 2022 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at https://mozilla.org/MPL/2.0/.

import { loadTimeData } from '../../common/loadTimeData'
import {
  HnsWallet,
  DefaultCurrencies
} from '../constants/types'
import { isComponentInStorybook } from '../utils/string-utils'

const marketUiOrigin = loadTimeData.getString('hnsWalletMarketUiBridgeUrl')
export const hnsWalletPanelOrigin = 'chrome://wallet-panel.top-chrome'

// remove trailing /
export const hnsMarketUiOrigin = marketUiOrigin.endsWith('/') ? marketUiOrigin.slice(0, -1) : marketUiOrigin
export const hnsWalletOrigin = 'chrome://wallet'

export const enum MarketUiCommand {
  UpdateCoinMarkets = 'update-coin-markets',
  SelectCoinMarket = 'select-coin-market',
  SelectBuy = 'select-buy',
  SelectDeposit = 'select-deposit',
  UpdateTradableAssets = 'update-tradable-assets',
  UpdateBuyableAssets = 'update-buyable-assets',
  UpdateDepositableAssets = 'update-depositable-assets',
  UpdateIframeHeight = 'update-iframe-height'
}

export type MarketCommandMessage = {
  command: MarketUiCommand
}

export type UpdateCoinMarketMessage = MarketCommandMessage & {
  payload: {
    coins: HnsWallet.CoinMarket[]
    defaultCurrencies: DefaultCurrencies
  }
}

export type SelectCoinMarketMessage = MarketCommandMessage & {
  payload: HnsWallet.CoinMarket
}

export type SelectBuyMessage = MarketCommandMessage & {
  payload: HnsWallet.CoinMarket
}

export type SelectDepositMessage = MarketCommandMessage & {
  payload: HnsWallet.CoinMarket
}

export type UpdateTradableAssetsMessage = MarketCommandMessage & {
  payload: HnsWallet.BlockchainToken[]
}

export type UpdateBuyableAssetsMessage = MarketCommandMessage & {
  payload: HnsWallet.BlockchainToken[]
}

export type UpdateDepositableAssetsMessage = MarketCommandMessage & {
  payload: HnsWallet.BlockchainToken[]
}

export type UpdateIframeHeightMessage =
  MarketCommandMessage & {
    payload: number
  }

export const sendMessageToMarketUiFrame = (targetWindow: Window | null, message: MarketCommandMessage) => {
  if (targetWindow && !isComponentInStorybook()) {
    targetWindow.postMessage(message, hnsMarketUiOrigin)
  }
}

export const sendMessageToWalletUi =
  (
    targetWindow: Window | null,
    message: MarketCommandMessage,
    origin: string
  ) => {
    if (targetWindow) {
      targetWindow.postMessage(message, origin)
    }
  }
