// Copyright (c) 2020 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at https://mozilla.org/MPL/2.0/.

import {RegisterPolymerComponentBehaviors} from 'chrome://resources/hns/polymer_overriding.js'

import {HnsResetProfileDialogBehavior} from '../hns_reset_page/hns_reset_profile_dialog_behavior.js'

RegisterPolymerComponentBehaviors({
  'settings-reset-profile-dialog': [
    HnsResetProfileDialogBehavior
  ]
})
