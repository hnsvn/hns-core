// Copyright (c) 2021 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at https://mozilla.org/MPL/2.0/.

(function() {
  if (!window.hnsEthereum) {
    return
  }
  var EventEmitter = require('events')
  var HnsWeb3ProviderEventEmitter = new EventEmitter()
  $Object.defineProperties(window.hnsEthereum, {
    on: {
      value: HnsWeb3ProviderEventEmitter.on,
      writable: false
    },
    emit: {
      value: HnsWeb3ProviderEventEmitter.emit,
      writable: false
    },
    removeListener: {
      value: HnsWeb3ProviderEventEmitter.removeListener,
      writable: false
    },
    removeAllListeners: {
      value: HnsWeb3ProviderEventEmitter.removeAllListeners,
      writable: false
    }
  })
})()
