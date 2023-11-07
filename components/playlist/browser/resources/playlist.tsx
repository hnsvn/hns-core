/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

import * as React from 'react'
import { render } from 'react-dom'
import { Provider } from 'react-redux'
import { BrowserRouter } from 'react-router-dom'

import { setIconBasePath } from '@hns/leo/react/icon'
import '@hns/leo/tokens/css/variables.css'

// Components
import App from './components/app.v1'

// Theme

import wireApiEventsToStore from './apiEventsToStore'
import startReceivingPlayerEvents from './playerEventSink'

// Utils
import store from './store'

function initialize () {
  render(
    <BrowserRouter>
      <Provider store={store}>
        <App />
      </Provider>
    </BrowserRouter>,
    document.getElementById('root')
  )
}

wireApiEventsToStore()
startReceivingPlayerEvents()
setIconBasePath('chrome-untrusted://resources/hns-icons')

document.addEventListener('DOMContentLoaded', initialize)
