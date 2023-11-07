// Copyright (c) 2019 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

import * as React from 'react'
import { Dispatch } from 'redux'
import { connect } from 'react-redux'

import '$web-components/app.global.scss'

// Components
import NewTabPage from './newTab'

// Utils
import * as PreferencesAPI from '../api/preferences'
import getHnsNewsController from '../api/hns_news'
import getNTPBrowserAPI from '../api/background'
import { getActionsForDispatch } from '../api/getActions'

// Types
import { NewTabActions } from '../constants/new_tab_types'
import { ApplicationState } from '../reducers'
import { HnsNewsState } from '../reducers/today'

interface Props {
  actions: NewTabActions
  newTabData: NewTab.State
  gridSitesData: NewTab.GridSitesState
  hnsNewsData: HnsNewsState
}

const getHnsNewsDisplayAd = function GetHnsNewsDisplayAd () {
  return getHnsNewsController().getDisplayAd()
}

function DefaultPage (props: Props) {
  const { newTabData, hnsNewsData, gridSitesData, actions } = props

  // don't render if user prefers an empty page
  if (props.newTabData.showEmptyPage && !props.newTabData.isIncognito) {
    return <div />
  }

  return (
    <NewTabPage
      newTabData={newTabData}
      todayData={hnsNewsData}
      gridSitesData={gridSitesData}
      actions={actions}
      saveShowBackgroundImage={PreferencesAPI.saveShowBackgroundImage}
      saveShowRewards={PreferencesAPI.saveShowRewards}
      saveShowHnsTalk={PreferencesAPI.saveShowHnsTalk}
      saveBrandedWallpaperOptIn={PreferencesAPI.saveBrandedWallpaperOptIn}
      saveSetAllStackWidgets={PreferencesAPI.saveSetAllStackWidgets}
      getHnsNewsDisplayAd={getHnsNewsDisplayAd}
      chooseNewCustomBackgroundImage={() => getNTPBrowserAPI().pageHandler.chooseLocalCustomBackground() }
      setCustomImageBackground={background => getNTPBrowserAPI().pageHandler.useCustomImageBackground(background) }
      removeCustomImageBackground={background => getNTPBrowserAPI().pageHandler.removeCustomImageBackground(background) }
      setHnsBackground={selectedBackground => getNTPBrowserAPI().pageHandler.useHnsBackground(selectedBackground)}
      setColorBackground={(color, useRandomColor) => getNTPBrowserAPI().pageHandler.useColorBackground(color, useRandomColor) }
    />
  )
}

const mapStateToProps = (state: ApplicationState): Partial<Props> => ({
  newTabData: state.newTabData,
  gridSitesData: state.gridSitesData,
  hnsNewsData: state.today
})

const mapDispatchToProps = (dispatch: Dispatch): Partial<Props> => {
  return {
    actions: getActionsForDispatch(dispatch)
  }
}

export default connect(
  mapStateToProps,
  mapDispatchToProps
)(DefaultPage)
