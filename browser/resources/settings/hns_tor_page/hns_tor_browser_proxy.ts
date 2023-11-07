// Copyright (c) 2022 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at https://mozilla.org/MPL/2.0/.

// @ts-nocheck TODO(petemill): Define types and remove ts-nocheck

import { sendWithPromise } from 'chrome://resources/js/cr.js';

export interface HnsTorBrowserProxy {
  getBridgesConfig(): Promise<any> // TODO(petemill): Define the expected type
  setBridgesConfig(config: any) // TODO(petemill): Define the expected type
  requestBridgesCaptcha(): Promise<any> // TODO(petemill): Define the expected type
  resolveBridgesCaptcha(captcha: any) // TODO(petemill): Define the expected type
  setTorEnabled(value: boolean)
  isTorEnabled(): Promise<boolean>
  isTorManaged(): Promise<boolean>
  isSnowflakeExtensionAllowed(): Promise<boolean>
  isSnowflakeExtensionEnabled(): Promise<boolean>
  enableSnowflakeExtension(enable: boolean): Promise<boolean>
}

export class HnsTorBrowserProxyImpl implements HnsTorBrowserProxy {
  static getInstance() {
    return instance || (instance = new HnsTorBrowserProxyImpl());
  }

  getBridgesConfig() {
    return sendWithPromise('hns_tor.getBridgesConfig')
  }

  setBridgesConfig(config) {
    chrome.send('hns_tor.setBridgesConfig', [config])
  }

  requestBridgesCaptcha() {
    return sendWithPromise('hns_tor.requestBridgesCaptcha')
  }

  resolveBridgesCaptcha(captcha) {
    return sendWithPromise('hns_tor.resolveBridgesCaptcha', captcha)
  }

  setTorEnabled(value) {
    chrome.send('hns_tor.setTorEnabled', [value])
  }

  isTorEnabled() {
    return sendWithPromise('hns_tor.isTorEnabled')
  }

  isTorManaged() {
    return sendWithPromise('hns_tor.isTorManaged')
  }

  isSnowflakeExtensionAllowed() {
    return sendWithPromise('hns_tor.isSnowflakeExtensionAllowed')
  }

  isSnowflakeExtensionEnabled(): Promise<boolean> {
    return sendWithPromise('hns_tor.isSnowflakeExtensionEnabled')
  }

  enableSnowflakeExtension(enable): Promise<boolean> {
    return sendWithPromise('hns_tor.enableSnowflakeExtension', enable)
  }
}

let instance: HnsTorBrowserProxy|null = null
