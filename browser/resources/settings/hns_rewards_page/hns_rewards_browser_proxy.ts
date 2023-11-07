/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * you can obtain one at https://mozilla.org/MPL/2.0/. */

// @ts-nocheck TODO(petemill): Define types and remove ts-nocheck

import { loadTimeData } from '../i18n_setup.js';

export interface HnsRewardsBrowserProxy {
  getRewardsEnabled(): Promise<boolean>
  wasInlineTipButtonsEnabledAtStartup(): boolean
  wasInlineTipTwitterEnabledAtStartup(): boolean
  wasInlineTipRedditEnabledAtStartup(): boolean
  wasInlineTipGithubEnabledAtStartup(): boolean
}

/**
 * @implements {settings.HnsRewardsBrowserProxy}
 */
export class HnsRewardsBrowserProxyImpl implements HnsRewardsBrowserProxy {
  getRewardsEnabled () {
    return new Promise<boolean>((resolve) => chrome.hnsRewards.getRewardsEnabled(
      (enabled) => { resolve(enabled) }))
  }

  wasInlineTipButtonsEnabledAtStartup() {
    return loadTimeData.getBoolean('inlineTipButtonsEnabled')
  }

  wasInlineTipTwitterEnabledAtStartup() {
    return loadTimeData.getBoolean('inlineTipTwitterEnabled')
  }

  wasInlineTipRedditEnabledAtStartup() {
    return loadTimeData.getBoolean('inlineTipRedditEnabled')
  }

  wasInlineTipGithubEnabledAtStartup() {
    return loadTimeData.getBoolean('inlineTipGithubEnabled')
  }

  static getInstance() {
    return instance || (instance = new HnsRewardsBrowserProxyImpl())
  }
}

let instance: HnsRewardsBrowserProxy|null = null
