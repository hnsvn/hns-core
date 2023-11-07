// Copyright (c) 2023 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

// Types
import { HnsWallet, DAppPermissionDurationOption } from '../constants/types'

export const DAppPermissionDurationOptions: DAppPermissionDurationOption[] = [
  {
    name: 'hnsWalletPermissionUntilClose',
    id: HnsWallet.PermissionLifetimeOption.kPageClosed
  },
  {
    name: 'hnsWalletPermissionOneDay',
    id: HnsWallet.PermissionLifetimeOption.k24Hours
  },
  {
    name: 'hnsWalletPermissionOneWeek',
    id: HnsWallet.PermissionLifetimeOption.k7Days
  },
  {
    name: 'hnsWalletPermissionForever',
    id: HnsWallet.PermissionLifetimeOption.kForever
  }
]
