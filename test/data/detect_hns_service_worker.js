/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

self.addEventListener('message', (event) => {
  if (event.data == 'isHns') {
    navigator.hns.isHns().then(value => {
      event.source.postMessage(value ? "HNS" : "NO");
    })
  }
});
