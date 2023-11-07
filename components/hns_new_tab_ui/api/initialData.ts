// Copyright (c) 2019 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

import * as preferencesAPI from './preferences'
import * as statsAPI from './stats'
import * as privateTabDataAPI from './privateTabData'
import * as wallpaper from './wallpaper'
import * as newTabAdsDataAPI from './newTabAdsData'
import getNTPBrowserAPI from './background'

export type InitialData = {
  preferences: NewTab.Preferences
  stats: statsAPI.Stats
  privateTabData: privateTabDataAPI.PrivateTabData
  wallpaperData?: NewTab.Wallpaper
  hnsBackgrounds: NewTab.HnsBackground[]
  customImageBackgrounds: NewTab.ImageBackground[]
  hnsRewardsSupported: boolean
  hnsTalkSupported: boolean
  searchPromotionEnabled: boolean
}

export type PreInitialRewardsData = {
  rewardsEnabled: boolean
  isGrandfatheredUser: boolean
  userType: string
  isUnsupportedRegion: boolean
  declaredCountry: string
  needsBrowserUpgradeToServeAds: boolean
}

export type InitialRewardsData = {
  report: NewTab.RewardsBalanceReport
  balance?: number
  externalWallet?: RewardsExtension.ExternalWallet
  externalWalletProviders?: string[]
  adsAccountStatement: NewTab.AdsAccountStatement
  parameters: NewTab.RewardsParameters
  publishersVisitedCount: number
}

const isIncognito: boolean = chrome.extension.inIncognitoContext

// Gets all data required for the first render of the page
export async function getInitialData (): Promise<InitialData> {
  try {
    console.timeStamp('Getting initial data...')
    const [
      preferences,
      stats,
      privateTabData,
      wallpaperData,
      hnsRewardsSupported,
      hnsTalkSupported,
      searchPromotionEnabled,
      hnsBackgrounds,
      customImageBackgrounds
    ] = await Promise.all([
      preferencesAPI.getPreferences(),
      statsAPI.getStats(),
      privateTabDataAPI.getPrivateTabData(),
      !isIncognito ? wallpaper.getWallpaper() : Promise.resolve(undefined),
      new Promise((resolve) => {
        chrome.hnsRewards.isSupported((supported: boolean) => {
          resolve(supported)
        })
      }),
      new Promise((resolve) => {
        if (!('hnsTalk' in chrome)) {
          resolve(false)
          return
        }

        chrome.hnsTalk.isSupported((supported: boolean) => {
          resolve(supported)
        })
      }),
      getNTPBrowserAPI().pageHandler.isSearchPromotionEnabled().then(({ enabled }) => enabled),
      getNTPBrowserAPI().pageHandler.getHnsBackgrounds().then(({ backgrounds }) => {
        return backgrounds.map(background => ({ type: 'hns', wallpaperImageUrl: background.imageUrl.url, author: background.author, link: background.link.url }))
      }),
      getNTPBrowserAPI().pageHandler.getCustomImageBackgrounds().then(({ backgrounds }) => {
        return backgrounds.map(background => ({ type: 'image', wallpaperImageUrl: background.url.url }))
      })
    ])
    console.timeStamp('Got all initial data.')
    return {
      preferences,
      stats,
      privateTabData,
      wallpaperData,
      hnsBackgrounds,
      customImageBackgrounds,
      hnsRewardsSupported,
      hnsTalkSupported,
      searchPromotionEnabled
    } as InitialData
  } catch (e) {
    console.error(e)
    throw Error('Error getting initial data')
  }
}

export async function getRewardsPreInitialData (): Promise<PreInitialRewardsData> {
  const [
    rewardsEnabled,
    isGrandfatheredUser,
    userType,
    isUnsupportedRegion,
    declaredCountry,
    adsData
  ] = await Promise.all([
    new Promise<boolean>(
      (resolve) => chrome.hnsRewards.getRewardsEnabled(resolve)),
    new Promise<boolean>(
      (resolve) => chrome.hnsRewards.isGrandfatheredUser(resolve)),
    new Promise<string>(
      (resolve) => chrome.hnsRewards.getUserType(resolve)),
    new Promise<boolean>(
      (resolve) => chrome.hnsRewards.isUnsupportedRegion(resolve)),
    new Promise<string>(
      (resolve) => chrome.hnsRewards.getDeclaredCountry(resolve)),
    newTabAdsDataAPI.getNewTabAdsData()
  ])

  const needsBrowserUpgradeToServeAds = adsData.needsBrowserUpgradeToServeAds

  return {
    rewardsEnabled,
    isGrandfatheredUser,
    userType,
    isUnsupportedRegion,
    declaredCountry,
    needsBrowserUpgradeToServeAds
  }
}

export async function getRewardsInitialData (): Promise<InitialRewardsData> {
  try {
    const [
      adsAccountStatement,
      report,
      balance,
      parameters,
      externalWallet,
      externalWalletProviders,
      publishersVisitedCount
    ] = await Promise.all([
      new Promise(resolve => chrome.hnsRewards.getAdsAccountStatement((success: boolean, adsAccountStatement: NewTab.AdsAccountStatement) => {
        resolve(success ? adsAccountStatement : undefined)
      })),
      new Promise(resolve => chrome.hnsRewards.getBalanceReport(new Date().getMonth() + 1, new Date().getFullYear(), (report: NewTab.RewardsBalanceReport) => {
        resolve(report)
      })),
      new Promise(resolve => chrome.hnsRewards.fetchBalance(
        (balance?: number) => {
          resolve(balance)
        }
      )),
      new Promise(resolve => chrome.hnsRewards.getRewardsParameters((parameters: NewTab.RewardsParameters) => {
        resolve(parameters)
      })),
      new Promise(resolve => {
        chrome.hnsRewards.getExternalWallet((wallet) => resolve(wallet))
      }),
      new Promise(resolve => {
        chrome.hnsRewards.getExternalWalletProviders(resolve)
      }),
      new Promise(resolve => {
        chrome.hnsRewards.getPublishersVisitedCount(resolve)
      }),
      new Promise(resolve => {
        chrome.hnsRewards.fetchPromotions(resolve)
      })
    ])
    return {
      adsAccountStatement,
      report,
      balance,
      parameters,
      externalWallet,
      externalWalletProviders,
      publishersVisitedCount
    } as InitialRewardsData
  } catch (err) {
    throw Error(err)
  }
}
