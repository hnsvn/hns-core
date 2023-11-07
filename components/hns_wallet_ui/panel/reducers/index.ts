/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

import { combineReducers } from 'redux'

import pageReducer from '../../page/reducers/page_reducer'
import accountsTabReducer from '../../page/reducers/accounts-tab-reducer'
import panelReducer from './panel_reducer'
import walletReducer from '../../common/slices/wallet.slice'
import sendCryptoReducer from '../../common/reducers/send_crypto_reducer'
import { walletApi } from '../../common/slices/api.slice'

export default combineReducers({
  page: pageReducer,
  panel: panelReducer,
  wallet: walletReducer,
  [walletApi.reducerPath]: walletApi.reducer,
  sendCrypto: sendCryptoReducer,
  accountsTab: accountsTabReducer
})
