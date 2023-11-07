// Copyright (c) 2021 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

// magics
import { SKIP_PRICE_LOOKUP_COINGECKO_ID } from '../common/constants/magics'

// types
import { HnsWallet, SupportedTestNetworks } from '../constants/types'

// options
import { AllNetworksOption } from './network-filter-options'

// utils
import { isComponentInStorybook } from '../utils/string-utils'

// icons
import {
  ETHIconUrl,
  SOLIconUrl,
  AVAXIconUrl,
  BNBIconUrl,
  BTCIconUrl,
  FILECOINIconUrl,
} from '../stories/mock-data/asset-icons'

const isStorybook = isComponentInStorybook()

export const getNetworkLogo = (chainId: string, symbol: string): string => {
  if (chainId === HnsWallet.AURORA_MAINNET_CHAIN_ID)
    return 'chrome://erc-token-images/aurora.png'
  if (chainId === HnsWallet.OPTIMISM_MAINNET_CHAIN_ID)
    return 'chrome://erc-token-images/op.png'
  if (chainId === HnsWallet.POLYGON_MAINNET_CHAIN_ID)
    return 'chrome://erc-token-images/matic.png'
  if (chainId === HnsWallet.BINANCE_SMART_CHAIN_MAINNET_CHAIN_ID)
    return isStorybook ? BNBIconUrl : 'chrome://erc-token-images/bnb.png'
  if (chainId === HnsWallet.AVALANCHE_MAINNET_CHAIN_ID)
    return isStorybook ? AVAXIconUrl : 'chrome://erc-token-images/avax.png'
  if (chainId === HnsWallet.FANTOM_MAINNET_CHAIN_ID)
    return 'chrome://erc-token-images/ftm.png'
  if (chainId === HnsWallet.CELO_MAINNET_CHAIN_ID)
    return 'chrome://erc-token-images/celo.png'
  if (chainId === HnsWallet.ARBITRUM_MAINNET_CHAIN_ID)
    return 'chrome://erc-token-images/arb.png'
  if (chainId === HnsWallet.NEON_EVM_MAINNET_CHAIN_ID)
    return 'chrome://erc-token-images/neon.png'
  if (chainId === HnsWallet.BASE_MAINNET_CHAIN_ID)
    return 'chrome://erc-token-images/base.png'
  if (chainId === AllNetworksOption.chainId)
    return AllNetworksOption.iconUrls[0]

  switch (symbol.toUpperCase()) {
    case 'SOL':
      return isStorybook ? SOLIconUrl : 'chrome://erc-token-images/sol.png'
    case 'ETH':
      return isStorybook ? ETHIconUrl : 'chrome://erc-token-images/eth.png'
    case 'FIL':
      return isStorybook ? FILECOINIconUrl : 'chrome://erc-token-images/fil.png'
    case 'BTC':
      return isStorybook ? BTCIconUrl : 'chrome://erc-token-images/btc.png'
  }

  return ''
}

export const makeNativeAssetLogo = (symbol: string, chainId: string) => {
  return getNetworkLogo(
    symbol.toUpperCase() === 'ETH'
      ? HnsWallet.MAINNET_CHAIN_ID
      : chainId,
    symbol
  )
}

type UndefinedIf<R, T> = T extends undefined ? undefined : R
export const makeNetworkAsset = <T extends HnsWallet.NetworkInfo | undefined>(
  network: T
): UndefinedIf<HnsWallet.BlockchainToken, T> => {
  if (network === undefined) {
    return undefined as UndefinedIf<HnsWallet.BlockchainToken, T>
  }

  return {
    contractAddress: '',
    name: network.symbolName,
    symbol: network.symbol,
    logo: makeNativeAssetLogo(network.symbol, network.chainId),
    isErc20: false,
    isErc721: false,
    isErc1155: false,
    isNft: false,
    isSpam: false,
    decimals: network.decimals,
    visible: true,
    tokenId: '',
    coingeckoId:
      // skip getting prices of known testnet tokens
      // except Goerli ETH, which has real-world value
      SupportedTestNetworks.includes(network.chainId) ?
      network.chainId === HnsWallet.GOERLI_CHAIN_ID &&
      network.symbol.toLowerCase() === 'eth'
        ? 'goerli-eth'
        : SKIP_PRICE_LOOKUP_COINGECKO_ID
      : '',
    chainId: network.chainId,
    coin: network.coin
  } as UndefinedIf<HnsWallet.BlockchainToken, T>
}
