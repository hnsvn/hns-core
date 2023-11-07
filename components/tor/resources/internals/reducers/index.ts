/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

import { combineReducers } from 'redux'

// Utils
import torInternalsReducer from './tor_internals_reducer'

export default combineReducers<TorInternals.ApplicationState>({
  torInternalsData: torInternalsReducer
})