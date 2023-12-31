// Copyright (c) 2021 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at https://mozilla.org/MPL/2.0/.

import * as React from 'react'
import { render } from 'react-dom'
import { Provider } from 'react-redux'
import { initLocale } from 'hns-ui'
import { setIconBasePath } from '@hns/leo/react/icon'

import '$web-components/app.global.scss'
import { loadTimeData } from '../../../common/loadTimeData'
import HnsCoreThemeProvider from '../../../common/HnsCoreThemeProvider'
import vpnDarkTheme from './theme/vpn-dark'
import vpnLightTheme from './theme/vpn-light'
import Container from './container'
import { PanelWrapper } from './style'
import store from './state/store'
import getPanelBrowserAPI from './api/panel_browser_api'

setIconBasePath('//resources/hns-icons')

function App () {
  React.useEffect(() => {
    const onVisibilityChange = () => {
      if (document.visibilityState === 'visible') {
        getPanelBrowserAPI().panelHandler.showUI()
      }
    }

    document.addEventListener('visibilitychange', onVisibilityChange)

    return () => {
      document.removeEventListener('visibilitychange', onVisibilityChange)
    }
  }, [])

  return (
    <Provider store={store}>
      <HnsCoreThemeProvider
        dark={vpnDarkTheme}
        light={vpnLightTheme}
      >
        <PanelWrapper>
          <Container />
        </PanelWrapper>
      </HnsCoreThemeProvider>
    </Provider>
  )
}

function initialize () {
  initLocale(loadTimeData.data_)
  render(<App />, document.getElementById('mountPoint'),
  () => {
    getPanelBrowserAPI().panelHandler.showUI()
  })
}

document.addEventListener('DOMContentLoaded', initialize)
