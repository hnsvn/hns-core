// Copyright (c) 2022 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at https://mozilla.org/MPL/2.0/.

import * as React from 'react'
import { render } from 'react-dom'
import { initLocale } from 'hns-ui'

import { loadTimeData } from '../../../common/loadTimeData'
import HnsCoreThemeProvider from '../../../common/HnsCoreThemeProvider'
import Container from './container'

function App () {
  return (
    <HnsCoreThemeProvider>
      <Container />
    </HnsCoreThemeProvider>
  )
}

function initialize () {
  initLocale(loadTimeData.data_)
  render(<App />, document.getElementById('mountPoint'))
}

document.addEventListener('DOMContentLoaded', initialize)
