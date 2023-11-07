// Copyright (c) 2023 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

import * as React from 'react'
import { render } from 'react-dom'
import { Provider } from 'react-redux'
import { BrowserRouter } from 'react-router-dom'

import { initLocale } from 'hns-ui'

// Style
import walletDarkTheme from '../../../../theme/wallet-dark'
import walletLightTheme from '../../../../theme/wallet-light'
import 'emptykit.css'

// Utils
import { loadTimeData } from '../../../../../common/loadTimeData'
import * as Lib from '../../../../common/async/lib'

// Actions
import * as WalletActions from '../../../../common/actions/wallet_actions'

// Components
import { store, walletPageApiProxy } from '../../../store'
import HnsCoreThemeProvider
  from '../../../../../common/HnsCoreThemeProvider'
import { FundWalletScreen } from '../../fund-wallet/fund-wallet'

// Hooks
import { LibContext } from '../../../../common/context/lib.context'
import { ApiProxyContext } from '../../../../common/context/api-proxy.context'

// Resources
import { setIconBasePath } from '@hns/leo/react/icon'
setIconBasePath('chrome://resources/hns-icons')

export function AndroidFundWalletApp() {
  return (
    <Provider store={store}>
      <BrowserRouter>
        <HnsCoreThemeProvider dark={walletDarkTheme} light={walletLightTheme}>
          <ApiProxyContext.Provider value={walletPageApiProxy}>
            <LibContext.Provider value={Lib}>
              <FundWalletScreen isAndroid={true} />
            </LibContext.Provider>
          </ApiProxyContext.Provider>
        </HnsCoreThemeProvider>
      </BrowserRouter>
    </Provider>
  )
}

function initialize() {
  initLocale(loadTimeData.data_)
  store.dispatch(WalletActions.initialize({}))
  render(AndroidFundWalletApp(), document.getElementById('root'))
}

document.addEventListener('DOMContentLoaded', initialize)
