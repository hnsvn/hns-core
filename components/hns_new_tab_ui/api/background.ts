/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

import * as HnsNewTabPage from 'gen/hns/components/hns_new_tab_ui/hns_new_tab_page.mojom.m.js'

// Provide access to all the generated types
export * from 'gen/hns/components/hns_new_tab_ui/hns_new_tab_page.mojom.m.js'

import { images as backgrounds, solidColorsForBackground, gradientColorsForBackground } from '../data/backgrounds'

/**
 * Generates a random image for new tab backgrounds
 */
export const randomBackgroundImage = (): NewTab.HnsBackground => {
  const randomIndex: number = Math.floor(Math.random() * backgrounds.length)
  const image: NewTab.HnsBackground = { ...backgrounds[randomIndex], random: true }
  return image
}

export const randomColorBackground = (color: string): NewTab.BackgroundWallpaper => {
  console.assert(color === HnsNewTabPage.RANDOM_SOLID_COLOR_VALUE || color === HnsNewTabPage.RANDOM_GRADIENT_COLOR_VALUE)

  const targetColors = color === HnsNewTabPage.RANDOM_SOLID_COLOR_VALUE ? solidColorsForBackground : gradientColorsForBackground
  const randomIndex: number = Math.floor(Math.random() * targetColors.length)
  const randomColor: NewTab.ColorBackground = {
    ...targetColors[randomIndex],
    random: true
  }
  return randomColor
}

interface API {
  pageCallbackRouter: HnsNewTabPage.PageCallbackRouter
  pageHandler: HnsNewTabPage.PageHandlerRemote
  addBackgroundUpdatedListener: (listener: BackgroundUpdated) => void
  addCustomImageBackgroundsUpdatedListener: (listener: CustomImageBackgroundsUpdated) => void
  addSearchPromotionDisabledListener: (listener: () => void) => void
}

type BackgroundUpdated = (background: HnsNewTabPage.Background) => void
type CustomImageBackgroundsUpdated = (backgrounds: HnsNewTabPage.CustomBackground[]) => void

let ntpBrowserAPIInstance: API

class NTPBrowserAPI implements API {
  pageCallbackRouter = new HnsNewTabPage.PageCallbackRouter()
  pageHandler = new HnsNewTabPage.PageHandlerRemote()

  constructor () {
    const factory = HnsNewTabPage.PageHandlerFactory.getRemote()
    factory.createPageHandler(
      this.pageCallbackRouter.$.bindNewPipeAndPassRemote(),
      this.pageHandler.$.bindNewPipeAndPassReceiver()
    )
  }

  addBackgroundUpdatedListener (listener: BackgroundUpdated) {
    this.pageCallbackRouter.onBackgroundUpdated.addListener(listener)
  }

  addCustomImageBackgroundsUpdatedListener (listener: CustomImageBackgroundsUpdated) {
    this.pageCallbackRouter.onCustomImageBackgroundsUpdated.addListener(listener)
  }

  addSearchPromotionDisabledListener (listener: () => void) {
    this.pageCallbackRouter.onSearchPromotionDisabled.addListener(listener)
  }
}

export default function getNTPBrowserAPI () {
  if (!ntpBrowserAPIInstance) {
    ntpBrowserAPIInstance = new NTPBrowserAPI()
  }
  return ntpBrowserAPIInstance
}
