// Copyright (c) 2021 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at https://mozilla.org/MPL/2.0/.

import * as React from 'react'
import { render } from 'react-dom'
import { initLocale } from 'hns-ui'

import { loadTimeData } from '../../../common/loadTimeData'
import HnsCoreThemeProvider from '../../../common/HnsCoreThemeProvider'
import shieldsDarkTheme from './theme/shields-dark'
import shieldsLightTheme from './theme/shields-light'
import { PanelWrapper } from './style'
import getPanelBrowserAPI from './api/panel_browser_api'
import Container from './container'
import { useSiteBlockInfoData, useSiteSettingsData } from './state/hooks'
import DataContext from './state/context'
import { ViewType } from './state/component_types'

function App () {
  const [initialThemeType, setInitialThemeType] = React.useState<chrome.hnsTheme.ThemeType>()
  const { siteBlockInfo } = useSiteBlockInfoData()
  const { siteSettings, getSiteSettings } = useSiteSettingsData()
  const [viewType, setViewType] = React.useState<ViewType>(ViewType.Main)

  const store = {
    siteBlockInfo,
    siteSettings,
    getSiteSettings,
    viewType,
    setViewType
  }

  React.useEffect(() => {
    chrome.hnsTheme.getHnsThemeType(setInitialThemeType)

    const onVisibilityChange = () => {
      if (document.visibilityState === 'visible') {
        getPanelBrowserAPI().panelHandler.showUI()
        getPanelBrowserAPI().dataHandler.updateFavicon()
        setViewType(ViewType.Main) /* Reset the view back to main panel */
      }
    }

    document.addEventListener('visibilitychange', onVisibilityChange)

    return () => {
      document.removeEventListener('visibilitychange', onVisibilityChange)
    }
  }, [])

  return (
    <>
      {initialThemeType &&
        <DataContext.Provider
          value={store}
        >
          <HnsCoreThemeProvider
            initialThemeType={initialThemeType}
            dark={shieldsDarkTheme}
            light={shieldsLightTheme}
          >
            <PanelWrapper>
              <Container />
            </PanelWrapper>
          </HnsCoreThemeProvider>
        </DataContext.Provider>
      }
    </>
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
