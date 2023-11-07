// Copyright (c) 2023 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

import * as React from 'react'
import { render } from 'react-dom'
import { Provider } from 'react-redux'

import { initLocale } from 'hns-ui'

// style
import walletDarkTheme from '../../../../theme/wallet-dark'
import walletLightTheme from '../../../../theme/wallet-light'
import 'emptykit.css'

// Utils
import { loadTimeData } from '../../../../../common/loadTimeData'
import * as Lib from '../../../../common/async/lib'

// actions
import * as WalletActions from '../../../../common/actions/wallet_actions'

// Components
import { store } from '../../../store'
import HnsCoreThemeProvider
  from '../../../../../common/HnsCoreThemeProvider'
import { Swap } from '../swap'
import { LibContext } from '../../../../common/context/lib.context'

export function AndroidSwapApp() {
  return (
    <Provider store={store}>
      <HnsCoreThemeProvider dark={walletDarkTheme} light={walletLightTheme}>
        <LibContext.Provider value={Lib}>
          <Swap />
        </LibContext.Provider>
      </HnsCoreThemeProvider>
    </Provider>
  )
}

function initialize () {
  initLocale(loadTimeData.data_)
  store.dispatch(WalletActions.initialize({}))
  render(AndroidSwapApp(), document.getElementById('root'))
}

document.addEventListener('DOMContentLoaded', initialize)
