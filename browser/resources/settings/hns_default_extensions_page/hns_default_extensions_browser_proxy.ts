// Copyright (c) 2022 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

import {sendWithPromise} from 'chrome://resources/js/cr.js'
import {loadTimeData} from '../i18n_setup.js';

export type ExtensionV2 = {
  id: string
  name: string
  description: string
  enabled: boolean
}

export interface HnsDefaultExtensionsBrowserProxy  {
  setWebTorrentEnabled(value: boolean): void
  setHangoutsEnabled(value: boolean): void
  setWidevineEnabled(value: boolean): void
  setMediaRouterEnabled(value: boolean): void
  isWidevineEnabled(): Promise<boolean>
  getRestartNeeded(): Promise<boolean>
  wasSignInEnabledAtStartup(): boolean
  isMediaRouterEnabled(): boolean
  getExtensionsManifestV2(): Promise<ExtensionV2[]>
  enableExtensionManifestV2(id: string, enabled: boolean): Promise<boolean>
}

export class HnsDefaultExtensionsBrowserProxyImpl implements HnsDefaultExtensionsBrowserProxy {
  setWebTorrentEnabled(value: boolean) {
    chrome.send('setWebTorrentEnabled', [value])
  }

  setHangoutsEnabled(value: boolean) {
    chrome.send('setHangoutsEnabled', [value])
  }

  setMediaRouterEnabled(value: boolean) {
    chrome.send('setMediaRouterEnabled', [value])
  }

  setWidevineEnabled(value: boolean) {
    chrome.send('setWidevineEnabled', [value])
  }

  isWidevineEnabled() {
    return sendWithPromise('isWidevineEnabled')
  }

  getRestartNeeded() {
    return sendWithPromise('getRestartNeeded')
  }

  wasSignInEnabledAtStartup() {
    return loadTimeData.getBoolean('signInAllowedOnNextStartupInitialValue')
  }

  isMediaRouterEnabled() {
    return loadTimeData.getBoolean('isMediaRouterEnabled')
  }

  getExtensionsManifestV2() {
    return sendWithPromise('getExtensionsManifestV2')
  }

  enableExtensionManifestV2(id: string, enabled: boolean) {
    return sendWithPromise('enableExtensionManifestV2', id, enabled);
  }

  static getInstance(): HnsDefaultExtensionsBrowserProxy {
    return instance || (instance = new HnsDefaultExtensionsBrowserProxyImpl())
  }
}

let instance: HnsDefaultExtensionsBrowserProxy|null = null
