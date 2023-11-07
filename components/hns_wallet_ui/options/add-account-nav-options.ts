// Copyright (c) 2021 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at https://mozilla.org/MPL/2.0/.
import { TopTabNavObjectType } from '../constants/types'
import { getLocale } from '../../common/locale'

export const AddAccountNavOptions = (): TopTabNavObjectType[] => [
  {
    id: 'create',
    name: getLocale('hnsWalletAddAccountCreate')
  },
  {
    id: 'import',
    name: getLocale('hnsWalletAddAccountImport')
  },
  {
    id: 'hardware',
    name: getLocale('hnsWalletAddAccountHardware')
  }
]
