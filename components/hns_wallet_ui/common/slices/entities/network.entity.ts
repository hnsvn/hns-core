// Copyright (c) 2022 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at https://mozilla.org/MPL/2.0/.

import {
  createDraftSafeSelector,
  createEntityAdapter,
  EntityAdapter,
  EntityId
} from '@reduxjs/toolkit'
import { HnsWallet } from '../../../constants/types'
import { getEntitiesListFromEntityState } from '../../../utils/entities.utils'

export type NetworkEntityAdaptor = EntityAdapter<HnsWallet.NetworkInfo> & {
  selectId: (network: {
    chainId: string
    coin: HnsWallet.CoinType
  }) => EntityId
}

export const networkEntityAdapter: NetworkEntityAdaptor =
  createEntityAdapter<HnsWallet.NetworkInfo>({
    selectId: ({ chainId, coin }): string =>
      chainId === HnsWallet.LOCALHOST_CHAIN_ID
        ? `${chainId}-${coin}`
        : chainId
  })

export type NetworksRegistry = ReturnType<
  (typeof networkEntityAdapter)['getInitialState']
> & {
  hiddenIds: string[]
  hiddenIdsByCoinType: Record<HnsWallet.CoinType, EntityId[]>
  idsByCoinType: Record<HnsWallet.CoinType, EntityId[]>
  mainnetIds: string[]
  onRampIds: string[]
  offRampIds: string[]
  visibleIds: string[]
}

export const emptyNetworksRegistry: NetworksRegistry = {
  ...networkEntityAdapter.getInitialState(),
  hiddenIds: [],
  hiddenIdsByCoinType: {},
  idsByCoinType: {},
  mainnetIds: [],
  onRampIds: [],
  offRampIds: [],
  visibleIds: []
}

//
// Selectors (From Query Results)
//
export const selectNetworksRegistryFromQueryResult = (result: {
  data?: NetworksRegistry
}) => {
  return result.data ?? emptyNetworksRegistry
}

export const {
  selectAll: selectAllNetworksFromQueryResult,
  selectById: selectNetworkByIdFromQueryResult,
  selectEntities: selectNetworkEntitiesFromQueryResult,
  selectIds: selectNetworkIdsFromQueryResult,
  selectTotal: selectTotalNetworksFromQueryResult,
} = networkEntityAdapter.getSelectors(selectNetworksRegistryFromQueryResult)

export const selectSwapSupportedNetworksFromQueryResult =
  createDraftSafeSelector(
    // inputs
    [
      selectNetworksRegistryFromQueryResult,
      (registry, swapSupportedIds: string[]) => swapSupportedIds
    ],
    // output
    (registry, swapSupportedIds) =>
      getEntitiesListFromEntityState(registry, swapSupportedIds)
  )

export const selectMainnetNetworksFromQueryResult = createDraftSafeSelector(
  // inputs
  [selectNetworksRegistryFromQueryResult],
  // output
  (registry) => getEntitiesListFromEntityState(registry, registry.mainnetIds)
)

export const selectOnRampNetworksFromQueryResult = createDraftSafeSelector(
  // inputs
  [selectNetworksRegistryFromQueryResult],
  // output
  (registry) => getEntitiesListFromEntityState(registry, registry.onRampIds)
)

export const selectOffRampNetworksFromQueryResult = createDraftSafeSelector(
  // inputs
  [selectNetworksRegistryFromQueryResult],
  // output
  (registry) => getEntitiesListFromEntityState(registry, registry.offRampIds)
)

export const selectVisibleNetworksFromQueryResult = createDraftSafeSelector(
  // inputs
  [selectNetworksRegistryFromQueryResult],
  // output
  (registry) => getEntitiesListFromEntityState(registry, registry.visibleIds)
)