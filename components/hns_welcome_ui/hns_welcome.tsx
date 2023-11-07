// Copyright (c) 2022 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at https://mozilla.org/MPL/2.0/.

import * as React from 'react'
import { render } from 'react-dom'
import { initLocale } from 'hns-ui'

import { loadTimeData } from '$web-common/loadTimeData'

import HnsCoreThemeProvider from '$web-common/HnsCoreThemeProvider'

import MainContainer from './main_container'
import DataContextProvider from './state/data-context-provider'

function App () {
  return (
    <DataContextProvider>
      <HnsCoreThemeProvider>
        <MainContainer />
      </HnsCoreThemeProvider>
    </DataContextProvider>
  )
}

function initialize () {
  initLocale(loadTimeData.data_)
  render(<App />, document.getElementById('root'),
  () => {})
}

document.addEventListener('DOMContentLoaded', initialize)
