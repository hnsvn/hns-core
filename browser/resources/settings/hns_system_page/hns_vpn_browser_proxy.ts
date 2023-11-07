// Copyright (c) 2023 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

import {sendWithPromise} from 'chrome://resources/js/cr.js';

export interface HnsVPNBrowserProxy {
  registerWireguardService(): Promise<boolean>;
  isWireguardServiceRegistered(): Promise<boolean>;
  isHnsVpnConnected(): Promise<boolean>;
}

export class HnsVPNBrowserProxyImpl implements HnsVPNBrowserProxy {
  registerWireguardService () {
    return sendWithPromise('registerWireguardService');
  }

  isWireguardServiceRegistered () {
    return sendWithPromise('isWireguardServiceRegistered');
  }

  isHnsVpnConnected () {
    return sendWithPromise('isHnsVpnConnected');
  }

  static getInstance(): HnsVPNBrowserProxy {
    return instance || (instance = new HnsVPNBrowserProxyImpl())
  }
}

let instance: HnsVPNBrowserProxy|null = null
