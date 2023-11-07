/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

import {sendWithPromise} from 'chrome://resources/js/cr.js';

/** @interface */
export interface HnsAppearanceBrowserProxy {
  /**
   * Returns JSON string with shape `chrome.hnsTheme.ThemeItem[]`
   */
  getHnsThemeList(): Promise<string>
  /**
   * Index of current ThemeItem
   */
  getHnsThemeType(): Promise<number>
  /**
   * 
   * @param value index of ThemeItem
   */
  setHnsThemeType(value: number): void
}

/**
 * @implements {HnsAppearanceBrowserProxy}
 */
export class HnsAppearanceBrowserProxyImpl implements
    HnsAppearanceBrowserProxy {
  getHnsThemeList() {
    return new Promise<string>(resolve => chrome.hnsTheme.getHnsThemeList(resolve))
  }

  getHnsThemeType() {
    return sendWithPromise('getHnsThemeType');
  }

  setHnsThemeType(value: number) {
    chrome.send('setHnsThemeType', [value]);
  }

  static getInstance(): HnsAppearanceBrowserProxyImpl {
    return instance || (instance = new HnsAppearanceBrowserProxyImpl())
  }
}

let instance: HnsAppearanceBrowserProxy|null = null
