// Copyright (c) 2023 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

import {
  UserType,
  userTypeFromString
} from '../../../hns_rewards/resources/shared/lib/user_type'

import {
  ExternalWallet,
  externalWalletFromExtensionData
} from '../../../hns_rewards/resources/shared/lib/external_wallet'

export const WalletStatus = {
  kNotConnected: 0,
  kConnected: 2,
  kLoggedOut: 4
} as const
export type WalletStatus = (typeof WalletStatus)[keyof typeof WalletStatus]

export type RewardsExternalWallet = Pick<
  ExternalWallet,
  'links' | 'provider' | 'username'
> & {
  status: WalletStatus
}

export class HnsRewardsProxy {
  getRewardsEnabled = () => {
    return new Promise<boolean>((resolve) =>
      chrome.hnsRewards.getRewardsEnabled((enabled) => {
        resolve(enabled)
      })
    )
  }

  fetchBalance = () => {
    return new Promise<number | undefined>((resolve) =>
      chrome.hnsRewards.fetchBalance((balance) => {
        resolve(balance)
      })
    )
  }

  getBalanceReport = (month: number, year: number) => {
    return new Promise<RewardsExtension.BalanceReport>((resolve) =>
      chrome.hnsRewards.getBalanceReport(month, year, (report) => {
        resolve(report)
      })
    )
  }

  getUserType = () => {
    return new Promise<UserType>((resolve) => {
      chrome.hnsRewards.getUserType((userType) => {
        resolve(userTypeFromString(userType))
      })
    })
  }

  getWalletExists = () => {
    return new Promise<boolean>((resolve) => {
      chrome.hnsRewards.getWalletExists((exists) => {
        resolve(exists)
      })
    })
  }

  getExternalWallet = () => {
    return new Promise<RewardsExternalWallet | null>((resolve) => {
      chrome.hnsRewards.getExternalWallet((data) => {
        const externalWallet = externalWalletFromExtensionData(data)
        const rewardsWallet: RewardsExternalWallet | null = externalWallet
          ? {
              ...externalWallet,
              status: externalWallet.status as WalletStatus
            }
          : null
        resolve(rewardsWallet)
      })
    })
  }

  isInitialized = () => {
    return new Promise<boolean>((resolve) => {
      chrome.hnsRewards.isInitialized((initialized) => {
        resolve(initialized)
      })
    })
  }

  isSupported = () => {
    return new Promise<boolean>((resolve) => {
      chrome.hnsRewards.isSupported((isSupported) => {
        resolve(isSupported)
      })
    })
  }

  onCompleteReset = chrome.hnsRewards.onCompleteReset.addListener

  onExternalWalletConnected =
    chrome.hnsRewards.onExternalWalletConnected.addListener

  onExternalWalletLoggedOut =
    chrome.hnsRewards.onExternalWalletLoggedOut.addListener

  onPublisherData = chrome.hnsRewards.onPublisherData.addListener

  onPublisherListNormalized =
    chrome.hnsRewards.onPublisherListNormalized.addListener

  onReconcileComplete = chrome.hnsRewards.onReconcileComplete.addListener

  onRewardsWalletCreated =
    chrome.hnsRewards.onRewardsWalletCreated.addListener

  onUnblindedTokensReady =
    chrome.hnsRewards.onUnblindedTokensReady.addListener

  openRewardsPanel = chrome.hnsRewards.openRewardsPanel
  showRewardsSetup = chrome.hnsRewards.showRewardsSetup

  onInitialized = (callback: () => any) =>
    chrome.hnsRewards.initialized.addListener((error) => {
      if (error === RewardsExtension.Result.OK) {
        callback()
      } else {
        console.error(`rewards onInitialized error: ${error}`)
      }
    })

  getAvailableCountries = () => {
    return new Promise<string[]>((resolve) =>
      chrome.hnsRewards.getAvailableCountries((countries) => {
        resolve(countries)
      })
    )
  }

  getRewardsParameters = () => {
    return new Promise<RewardsExtension.RewardsParameters>((resolve) =>
      chrome.hnsRewards.getRewardsParameters((params) => {
        resolve(params)
      })
    )
  }

  getAllNotifications = () => {
    return new Promise<RewardsExtension.Notification[]>((resolve) => {
      chrome.hnsRewards.getAllNotifications((notifications) => {
        resolve(notifications)
      })
    })
  }
}

export type HnsRewardsProxyInstance = InstanceType<typeof HnsRewardsProxy>

let hnsRewardsProxyInstance: HnsRewardsProxyInstance

export const getHnsRewardsProxy = () => {
  if (!hnsRewardsProxyInstance) {
    hnsRewardsProxyInstance = new HnsRewardsProxy()
  }

  return hnsRewardsProxyInstance
}
