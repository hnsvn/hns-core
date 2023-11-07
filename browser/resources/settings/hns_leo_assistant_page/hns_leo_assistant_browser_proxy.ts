/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

 import {sendWithPromise} from 'chrome://resources/js/cr.js';

 export interface HnsLeoAssistantBrowserProxy {
  resetLeoData(): void
  getLeoIconVisibility(): Promise<boolean>
  toggleLeoIcon(): void
 }

 export class HnsLeoAssistantBrowserProxyImpl
    implements HnsLeoAssistantBrowserProxy {

   static getInstance(): HnsLeoAssistantBrowserProxyImpl {
     return instance || (instance = new HnsLeoAssistantBrowserProxyImpl())
   }

  getLeoIconVisibility() {
    return sendWithPromise('getLeoIconVisibility')
  }

  toggleLeoIcon() {
    chrome.send('toggleLeoIcon')
  }

  resetLeoData() {
    chrome.send('resetLeoData')
  }
 }

 let instance: HnsLeoAssistantBrowserProxyImpl|null = null
