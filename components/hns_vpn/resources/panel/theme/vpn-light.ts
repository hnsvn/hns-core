/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

import ITheme from 'hns-ui/theme/theme-interface'
import IThemeVPN from './vpn-theme'
import defaultTheme from 'hns-ui/theme/hns-default'

const vpnLightTheme: ITheme & IThemeVPN = {
  ...defaultTheme,
  name: 'VPN Light',
  color: {
    ...defaultTheme.color,
    panelBackground: 'linear-gradient(180deg, #FFFFFF 58.56%, #F8F8FF 100%)',
    warningBackground: '#FFFCF0'
  }
}

export default vpnLightTheme
