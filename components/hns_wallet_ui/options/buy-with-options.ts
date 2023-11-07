/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

import { getLocale } from '$web-common/locale'
import { HnsWallet, BuyOption } from '../constants/types'

import RampIcon from '../assets/svg-icons/ramp-icon.svg'
import SardineIconLight from '../assets/svg-icons/sardine-logo-light.svg'
import SardineIconDark from '../assets/svg-icons/sardine-logo-dark.svg'
import TransakIcon from '../assets/svg-icons/transak-logo.svg'
import StripeIcon from '../assets/svg-icons/stripe-logo.svg'
import CoinbaseIcon from '../assets/svg-icons/coinbase-logo.svg'
import { isStripeSupported } from '../utils/asset-utils'

function getBuyOptions (): BuyOption[] {
  const buyOptions = [{
    id: HnsWallet.OnRampProvider.kRamp,
    actionText: getLocale('hnsWalletBuyWithRamp'),
    icon: RampIcon,
    name: getLocale('hnsWalletBuyRampNetworkName'),
    description: getLocale('hnsWalletBuyRampDescription')
  },
  {
    id: HnsWallet.OnRampProvider.kTransak,
    actionText: getLocale('hnsWalletBuyWithTransak'),
    icon: TransakIcon,
    name: getLocale('hnsWalletBuyTransakName'),
    description: getLocale('hnsWalletBuyTransakDescription')
  },
  {
    id: HnsWallet.OnRampProvider.kSardine,
    actionText: getLocale('hnsWalletBuyWithSardine'),
    icon: window.matchMedia('(prefers-color-scheme: dark)').matches
      ? SardineIconDark
      : SardineIconLight,
    name: getLocale('hnsWalletBuySardineName'),
    description: getLocale('hnsWalletBuySardineDescription')
  },
  {
    id: HnsWallet.OnRampProvider.kCoinbase,
    actionText: getLocale('hnsWalletBuyWithCoinbase'),
    icon: CoinbaseIcon,
    name: getLocale('hnsWalletBuyCoinbaseName'),
    description: getLocale('hnsWalletBuyCoinbaseDescription')
  }
]

  if(isStripeSupported()) {
    buyOptions.push({
      id: HnsWallet.OnRampProvider.kStripe,
      actionText: getLocale('hnsWalletBuyWithStripe'),
      icon: StripeIcon,
      name: getLocale('hnsWalletBuyStripeName'),
      description: getLocale('hnsWalletBuyStripeDescription')
    })
  }

  return buyOptions
}

export const BuyOptions = getBuyOptions()
