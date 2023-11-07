// Copyright (c) 2022 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at https://mozilla.org/MPL/2.0/.

import {sendWithPromise} from 'chrome://resources/js/cr.js';

export interface HnsSearchEnginesPageBrowserProxy {
  getPrivateSearchEnginesList(): Promise<any[]> // TODO(petemill): Define the expected type
}
 
export class HnsSearchEnginesPageBrowserProxyImpl implements HnsSearchEnginesPageBrowserProxy {
  getPrivateSearchEnginesList() {
    return sendWithPromise('getPrivateSearchEnginesList');
  }
  static getInstance() {
    return instance || (instance = new HnsSearchEnginesPageBrowserProxyImpl())
  }
}

let instance: HnsSearchEnginesPageBrowserProxy|null = null
