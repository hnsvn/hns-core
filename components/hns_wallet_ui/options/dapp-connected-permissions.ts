// Copyright (c) 2023 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

// Types
import { DAppConnectedPermissionsOption } from '../constants/types'

export const DAppPermittedOptions: DAppConnectedPermissionsOption[] = [
  {
    name: 'hnsWalletConnectPermissionBalanceAndActivity'
  },
  {
    name: 'hnsWalletConnectPermissionRequestApproval'
  },
  {
    name: 'hnsWalletConnectPermissionAddress'
  }
]

export const DAppNotPermittedOptions: DAppConnectedPermissionsOption[] = [
  {
    name: 'hnsWalletConnectPermissionMoveFunds'
  }
]
