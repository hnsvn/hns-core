// Copyright (c) 2019 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

import * as React from 'react'
import { render } from 'react-dom'
import { Provider } from 'react-redux'
import Theme from 'hns-ui/theme/hns-default'
import DarkTheme from 'hns-ui/theme/hns-dark'
import '../common/defaultTrustedTypesPolicy'
import HnsCoreThemeProvider from '../common/HnsCoreThemeProvider'
import { wireApiEventsToStore } from './apiEventsToStore'
import * as topSitesAPI from './api/topSites'
import { init } from './actions/new_tab_actions'

// Components
import App from './containers/app'
import { RewardsContextAdapter } from './components/default/rewards'

// Utils
import store from './store'

// Let things handle 'init'
store.dispatch(init())

function initialize () {
  console.timeStamp('loaded')
  // Get rendering going
  new Promise(resolve => chrome.hnsTheme.getHnsThemeType(resolve))
  .then((themeType: chrome.hnsTheme.ThemeType) => {
    render(
      <Provider store={store}>
        <HnsCoreThemeProvider
          initialThemeType={themeType}
          dark={DarkTheme}
          light={Theme}
        >
          <RewardsContextAdapter>
            <App />
          </RewardsContextAdapter>
        </HnsCoreThemeProvider>
      </Provider>,
      document.getElementById('root'),
      () => console.timeStamp('first react render'))
  })
  .catch((error) => {
    console.error('Problem mounting hns new tab', error)
  })
}

console.timeStamp('JS start')

// Get store data going
wireApiEventsToStore()

// Perform DOM-dependent initialization when ready
document.addEventListener('DOMContentLoaded', initialize)

// Update topsite tiles when NTP gets visible.
document.addEventListener('visibilitychange', () => {
  if (document.visibilityState === 'visible') {
    topSitesAPI.updateMostVisitedInfo()
  }
})
