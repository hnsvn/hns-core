// Copyright (c) 2020 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at https://mozilla.org/MPL/2.0/.

import * as React from 'react'
import { render } from 'react-dom'
import { initLocale } from 'hns-ui'

import 'emptykit.css'

import LegacyApp from './components/legacy_app'
import Theme from 'hns-ui/theme/hns-default'
import DarkTheme from 'hns-ui/theme/hns-dark'
import HnsCoreThemeProvider from '../../../../components/common/HnsCoreThemeProvider'
import { loadTimeData } from '../../../../components/common/loadTimeData'

function initialize () {
  chrome.hnsWallet.shouldPromptForSetup((prompt: boolean) => {
    if (!prompt) {
      chrome.hnsWallet.loadUI(() => {
        window.location.href = 'chrome://wallet'
      })
      return
    }
    renderUI()
  })
}

function renderUI () {
  new Promise(resolve => chrome.hnsTheme.getHnsThemeType(resolve))
  .then((themeType: chrome.hnsTheme.ThemeType) => {
    initLocale(loadTimeData.data_)

    render(
      <HnsCoreThemeProvider
        initialThemeType={themeType}
        dark={DarkTheme}
        light={Theme}
      >
        <LegacyApp />
      </HnsCoreThemeProvider>,
      document.getElementById('root')
    )
  })
  .catch(({ message }) => {
    console.error(`Could not mount hns wallet: ${message}`)
  })
}

document.addEventListener('DOMContentLoaded', initialize)
