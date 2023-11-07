// Copyright (c) 2021 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at https://mozilla.org/MPL/2.0/.

import * as React from 'react'

// types
import { HnsWallet, BlockExplorerUrlTypes } from '../../constants/types'

// utils
import { openBlockExplorerURL } from '../../utils/block-explorer-utils'

export function useExplorer (
  network?: Pick<HnsWallet.NetworkInfo, 'chainId' | 'blockExplorerUrls'>
) {
  return React.useCallback(
    (type: BlockExplorerUrlTypes, value?: string, id?: string) =>
      openBlockExplorerURL({
        network,
        value,
        type,
        id
      }),
    [network]
  )
}

export default useExplorer
