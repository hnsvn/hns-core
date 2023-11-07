/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

import ITheme from 'hns-ui/theme/theme-interface'
import IThemeWallet from './wallet-theme'
import defaultDarkTheme from 'hns-ui/theme/hns-dark'
import colors from 'hns-ui/theme/colors'

const walletDarkTheme: ITheme & IThemeWallet = {
  ...defaultDarkTheme,
  name: 'Wallet Dark',
  color: {
    ...defaultDarkTheme.color,
    text: colors.white,
    panelBackground: colors.grey900,
    outlineColor: 'rgba(255,255,255,0.5)'
  }
}

export default walletDarkTheme
