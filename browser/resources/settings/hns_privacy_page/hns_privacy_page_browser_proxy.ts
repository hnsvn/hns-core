// Copyright (c) 2022 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at https://mozilla.org/MPL/2.0/.

import {sendWithPromise} from 'chrome://resources/js/cr.js';
import {loadTimeData} from '../i18n_setup.js';

export interface HnsPrivacyBrowserProxy {
  getP3AEnabled(): Promise<boolean>
  setP3AEnabled(value: boolean): void
  getStatsUsagePingEnabled(): Promise<boolean>
  setStatsUsagePingEnabled(value: boolean): void
  wasPushMessagingEnabledAtStartup(): boolean
}

export class HnsPrivacyBrowserProxyImpl implements HnsPrivacyBrowserProxy {
  getP3AEnabled(): Promise<boolean> {
    return sendWithPromise('getP3AEnabled');
  }

  setP3AEnabled(value: boolean): void {
    chrome.send('setP3AEnabled', [value])
  }

  getStatsUsagePingEnabled(): Promise<boolean> {
    return sendWithPromise('getStatsUsagePingEnabled');
  }

  setStatsUsagePingEnabled(value: boolean): void {
    chrome.send('setStatsUsagePingEnabled', [value])
  }

  wasPushMessagingEnabledAtStartup(): boolean {
    return loadTimeData.getBoolean('pushMessagingEnabledAtStartup');
  }

  static getInstance(): HnsPrivacyBrowserProxyImpl {
    return instance || (instance = new HnsPrivacyBrowserProxyImpl())
  }
}

let instance: HnsPrivacyBrowserProxy | null = null
