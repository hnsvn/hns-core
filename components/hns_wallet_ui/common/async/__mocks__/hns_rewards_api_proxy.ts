// Copyright (c) 2023 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

import type {
  HnsRewardsProxyInstance,
  RewardsExternalWallet
} from '../hns_rewards_api_proxy'

export type HnsRewardsProxyOverrides = Partial<{
  rewardsEnabled: boolean
  balance: number
  externalWallet: RewardsExternalWallet | null
}>

export class MockHnsRewardsProxy {
  overrides: HnsRewardsProxyOverrides = {
    rewardsEnabled: true,
    balance: 100.5,
    externalWallet: null
  }

  applyOverrides = (overrides?: HnsRewardsProxyOverrides) => {
    if (!overrides) {
      return
    }

    this.overrides = { ...this.overrides, ...overrides }
  }

  getRewardsEnabled = async () => {
    return this.overrides.rewardsEnabled
  }

  fetchBalance = async () => {
    return this.overrides.balance
  }

  getExternalWallet = async () => {
    return this.overrides.externalWallet
  }
}

export type MockHnsRewardsProxyInstance = InstanceType<
  typeof MockHnsRewardsProxy
>

let hnsRewardsProxyInstance: MockHnsRewardsProxyInstance

export const getMockedHnsRewardsProxy = () => {
  if (!hnsRewardsProxyInstance) {
    hnsRewardsProxyInstance = new MockHnsRewardsProxy()
  }

  return hnsRewardsProxyInstance as unknown as HnsRewardsProxyInstance &
    MockHnsRewardsProxy
}
