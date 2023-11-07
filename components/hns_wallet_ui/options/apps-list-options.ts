// Copyright (c) 2021 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at https://mozilla.org/MPL/2.0/.
import { AppsListType } from '../constants/types'
import { getLocale } from '../../common/locale'
const compoundIcon = require('../assets/app-icons/compound-icon.png')
const makerIcon = require('../assets/app-icons/maker-icon.jpeg')
const aaveIcon = require('../assets/app-icons/aave-icon.jpeg')
const openSeaIcon = require('../assets/app-icons/opensea-icon.png')
const raribleIcon = require('../assets/app-icons/rarible-icon.png')

export const AppsList = (): AppsListType[] => [
  {
    category: getLocale('hnsWalletDefiCategory'),
    categoryButtonText: getLocale('hnsWalletDefiButtonText'),
    appList: [
      {
        name: getLocale('hnsWalletCompoundName'),
        description: getLocale('hnsWalletCompoundDescription'),
        url: '',
        icon: compoundIcon
      },
      {
        name: getLocale('hnsWalletMakerName'),
        description: getLocale('hnsWalletMakerDescription'),
        url: '',
        icon: makerIcon
      },
      {
        name: getLocale('hnsWalletAaveName'),
        description: getLocale('hnsWalletAaveDescription'),
        url: '',
        icon: aaveIcon
      }
    ]
  },
  {
    category: getLocale('hnsWalletNftCategory'),
    categoryButtonText: getLocale('hnsWalletNftButtonText'),
    appList: [
      {
        name: getLocale('hnsWalletOpenSeaName'),
        description: getLocale('hnsWalletOpenSeaDescription'),
        url: '',
        icon: openSeaIcon
      },
      {
        name: getLocale('hnsWalletRaribleName'),
        description: getLocale('hnsWalletRaribleDescription'),
        url: '',
        icon: raribleIcon
      }
    ]
  }
]
