// Copyright (c) 2023 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

// Types
import {
  HnsWallet,
  SupportedOffRampNetworks
} from '../../../constants/types'
import {
  WalletApiEndpointBuilderParams
} from '../api-base.slice'

// Utils
import {
  getBatTokensFromList,
  getNativeTokensFromList,
  getUniqueAssets
} from '../../../utils/asset-utils'
import { addLogoToToken } from '../../async/lib'
import { mapLimit } from 'async'

export const offRampEndpoints = ({
  query
}: WalletApiEndpointBuilderParams) => {
  return {
    getOffRampAssets: query<
      {
        rampAssetOptions: HnsWallet.BlockchainToken[]
        allAssetOptions: HnsWallet.BlockchainToken[]
      },
      void
    >({
      queryFn: async (_arg, _store, _extraOptions, baseQuery) => {
        try {
          const {
            data: { blockchainRegistry }
          } = baseQuery(undefined)
          const { kRamp } = HnsWallet.OffRampProvider

          const rampAssets = await mapLimit(
            SupportedOffRampNetworks,
            10,
            async (chainId: string) =>
              await blockchainRegistry.getSellTokens(kRamp, chainId)
          )

          // add token logos
          const rampAssetOptions: HnsWallet.BlockchainToken[] =
            await mapLimit(
              rampAssets.flatMap((p) => p.tokens),
              10,
              async (token: HnsWallet.BlockchainToken) =>
                await addLogoToToken(token)
            )

          // separate native assets from tokens
          const {
            tokens: rampTokenOptions,
            nativeAssets: rampNativeAssetOptions
          } = getNativeTokensFromList(rampAssetOptions)

          // separate BAT from other tokens
          const {
            bat: rampBatTokens,
            nonBat: rampNonBatTokens
          } = getBatTokensFromList(rampTokenOptions)

          // moves Gas coins and BAT to front of list
          const sortedRampOptions =
            [
              ...rampNativeAssetOptions,
              ...rampBatTokens,
              ...rampNonBatTokens
            ]

          const results = {
            rampAssetOptions: sortedRampOptions,
            allAssetOptions: getUniqueAssets([
              ...sortedRampOptions
            ])
          }

          return {
            data: results
          }
        } catch (error) {
          const errorMessage = `Unable to fetch offRamp assets: ${error}`
          console.log(errorMessage)
          return {
            error: errorMessage
          }
        }
      }, providesTags: (_results, error, _arg) => {
        if (error) {
          return ['UNKNOWN_ERROR']
        }
        return ['OffRampAssets']
      }
    })
  }
}
