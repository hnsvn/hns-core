// Copyright 2020 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at https://mozilla.org/MPL/2.0/.

// @ts-nocheck TODO(petemill): Convert to Polymer class and remove ts-nocheck

import {Polymer} from 'chrome://resources/polymer/v3_0/polymer/polymer_bundled.min.js';
import {getTemplate} from './hns_history_item.html.js'

Polymer({
  is: 'hns-history-item',
  _template: getTemplate()
})
