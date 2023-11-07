/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

// Import HnsNewsControllerMock first.
import './default/data/mockHnsNewsController'

import * as React from 'react'
import { Dispatch } from 'redux'
import { Provider as ReduxProvider } from 'react-redux'
// Components
import NewTabPage from '../containers/newTab'
import { getActionsForDispatch } from '../api/getActions'
import store from '../store'
import { useNewTabData, getGridSitesData } from './default/data/storybookState'
import { onChangeColoredBackground, onUseHnsBackground, onShowBrandedImageChanged } from './default/data/backgroundWallpaper'
import getTodayState from './default/data/todayStorybookState'
import getHnsNewsDisplayAd from './default/data/getHnsNewsDisplayAd'
import { getDataUrl, getUnpaddedAsDataUrl } from '../../common/privateCDN'
import { images, updateImages } from '../data/backgrounds'

const doNothingDispatch: Dispatch = (action: any) => action

function getActions () {
  return getActionsForDispatch(doNothingDispatch)
}

// @ts-expect-error
window.hnsStorybookUnpadUrl = async function UnpadUrl (paddedUrl: string, mimeType = 'image/jpg'): Promise<string> {
  const response = await fetch(paddedUrl)
  const blob = await response.blob()
  const buffer = await blob.arrayBuffer()
  if (paddedUrl.endsWith('.pad')) {
    return await getUnpaddedAsDataUrl(buffer, mimeType)
  }
  // Image is already unpadded
  return await getDataUrl(buffer)
}

const StoreProvider: React.FunctionComponent = ({ children }) => {
  return (
    <ReduxProvider store={store}>
     {children}
    </ReduxProvider>
  )
}

export default {
  title: 'New Tab',
  decorators: [
    (Story: any) => <StoreProvider><Story /></StoreProvider>
  ]
}

updateImages(images.map((image): NewTab.HnsBackground => {
  return {
    ...image,
    wallpaperImageUrl: require('../../img/newtab/backgrounds/' + image.wallpaperImageUrl)
  }
}))

export const Regular = () => {
  const doNothing = (value?: any) => value
  const state = store.getState()
  const newTabData = useNewTabData(state.newTabData)
  const gridSitesData = getGridSitesData(state.gridSitesData)
  const todayState = getTodayState()

  return (
    <NewTabPage
      newTabData={newTabData}
      todayData={todayState}
      gridSitesData={gridSitesData}
      actions={getActions()}
      saveShowBackgroundImage={doNothing}
      saveShowRewards={doNothing}
      saveShowHnsTalk={doNothing}
      saveBrandedWallpaperOptIn={onShowBrandedImageChanged}
      saveSetAllStackWidgets={doNothing}
      getHnsNewsDisplayAd={getHnsNewsDisplayAd}
      setHnsBackground={onUseHnsBackground}
      chooseNewCustomBackgroundImage={doNothing}
      setCustomImageBackground={doNothing}
      removeCustomImageBackground={doNothing}
      setColorBackground={onChangeColoredBackground}
    />
  )
}
