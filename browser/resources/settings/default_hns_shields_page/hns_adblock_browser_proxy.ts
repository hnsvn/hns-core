/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

// @ts-nocheck TODO(petemill): Define types and remove ts-nocheck

import { sendWithPromise, addWebUiListener } from 'chrome://resources/js/cr.js';

export interface HnsAdblockBrowserProxy {
  getRegionalLists(): Promise<any[]> // TODO(petemill): Define the expected type
  enableFilterList(uuid: string, enabled: boolean)
  getListSubscriptions(): Promise<any> // TODO(petemill): Define the expected type
  getCustomFilters(): Promise<any> // TODO(petemill): Define the expected type
  setSubscriptionEnabled(url: string, enabled: boolean)
  addSubscription(url: string)
  addWebUiListener(eventName: string, callback: Function)
  updateSubscription(url: string)
  deleteSubscription(url: string)
  viewSubscription(url: string)
}

export class HnsAdblockBrowserProxyImpl implements HnsAdblockBrowserProxy {
  static getInstance() {
    return instance || (instance = new HnsAdblockBrowserProxyImpl());
  }

  /** @returns {Promise} */
  getRegionalLists () {
    return sendWithPromise('hns_adblock.getRegionalLists')
  }

  /** @returns {Promise} */
  getListSubscriptions () {
    return sendWithPromise('hns_adblock.getListSubscriptions')
  }

  /** @returns {Promise} */
  getCustomFilters () {
    return sendWithPromise('hns_adblock.getCustomFilters')
  }

  enableFilterList (uuid, enabled) {
    chrome.send('hns_adblock.enableFilterList', [uuid, enabled])
  }

  setSubscriptionEnabled (url, enabled) {
    chrome.send('hns_adblock.setSubscriptionEnabled', [url, enabled])
  }

  addSubscription (url) {
    chrome.send('hns_adblock.addSubscription', [url])
  }

  updateSubscription (url) {
    chrome.send('hns_adblock.updateSubscription', [url])
  }

  updateCustomFilters (value) {
    chrome.send('hns_adblock.updateCustomFilters', [value])
  }

  deleteSubscription (url) {
    chrome.send('hns_adblock.deleteSubscription', [url])
  }

  viewSubscription (url) {
    chrome.send('hns_adblock.viewSubscription', [url])
  }

  addWebUiListener (event_name, callback) {
    addWebUiListener(event_name, callback)
  }
}

let instance: HnsAdblockBrowserProxy|null = null
