/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

import ITheme from 'hns-ui/theme/theme-interface'
import IThemeWelcomePage from './welcome-theme'
import defaultTheme from 'hns-ui/theme/hns-default'
import colors from 'hns-ui/theme/colors'

const welcomeLightTheme: ITheme & IThemeWelcomePage = {
  ...defaultTheme,
  name: 'Welcome Light',
  color: {
    ...defaultTheme.color,
    text: colors.neutral900,
    panelBackground: '#F9F9FD',
    panelBackgroundSecondary: colors.neutral000,
    outlineColor: 'rgba(255,80,0,0.2)'
  }
}

export default welcomeLightTheme
