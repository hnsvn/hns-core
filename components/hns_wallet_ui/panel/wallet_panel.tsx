// Copyright (c) 2021 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at https://mozilla.org/MPL/2.0/.

import * as React from 'react'
import { render } from 'react-dom'
import { Provider } from 'react-redux'
import { initLocale } from 'hns-ui'
import { loadTimeData } from '../../common/loadTimeData'
import walletDarkTheme from '../theme/wallet-dark'
import walletLightTheme from '../theme/wallet-light'
import HnsCoreThemeProvider from '../../common/HnsCoreThemeProvider'
import store, { walletPanelApiProxy } from './store'
import * as WalletActions from '../common/actions/wallet_actions'
import Container from './container'
import { LibContext } from '../common/context/lib.context'
import * as Lib from '../common/async/lib'
import { ApiProxyContext } from '../common/context/api-proxy.context'
import { setIconBasePath } from '@hns/leo/react/icon'
import { removeDeprecatedLocalStorageKeys } from '../common/constants/local-storage-keys'
setIconBasePath('chrome://resources/hns-icons')

function App () {
  const [initialThemeType, setInitialThemeType] = React.useState<chrome.hnsTheme.ThemeType>()
  React.useEffect(() => {
    chrome.hnsTheme.getHnsThemeType(setInitialThemeType)
  }, [])

  React.useEffect(() => {
    removeDeprecatedLocalStorageKeys()
  }, [])

  return (
    <Provider store={store}>
      {initialThemeType &&
        <HnsCoreThemeProvider
          initialThemeType={initialThemeType}
          dark={walletDarkTheme}
          light={walletLightTheme}
        >
          <ApiProxyContext.Provider value={walletPanelApiProxy}>
            <LibContext.Provider value={Lib}>
              <Container />
            </LibContext.Provider>
          </ApiProxyContext.Provider>
        </HnsCoreThemeProvider>
      }
    </Provider>
  )
}

function initialize () {
  initLocale(loadTimeData.data_)
  store.dispatch(WalletActions.initialize({
    skipBalancesRefresh: true
  }))
  render(<App />, document.getElementById('mountPoint'))
}

document.addEventListener('DOMContentLoaded', initialize)
