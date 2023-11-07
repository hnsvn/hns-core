// Copyright (c) 2021 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at https://mozilla.org/MPL/2.0/.
import { HnsWallet } from '../../constants/types'
import {
  BTCIconUrl,
  ETHIconUrl,
  FILECOINIconUrl,
  SOLIconUrl
} from './asset-icons'

export const mockEthMainnet: HnsWallet.NetworkInfo = {
  activeRpcEndpointIndex: 0,
  blockExplorerUrls: ['https://etherscan.io', 'https://etherchain.org'],
  chainId: HnsWallet.MAINNET_CHAIN_ID,
  chainName: 'Ethereum Mainnet',
  coin: HnsWallet.CoinType.ETH,
  supportedKeyrings: [HnsWallet.KeyringId.kDefault],
  decimals: 18,
  iconUrls: [ETHIconUrl],
  isEip1559: true,
  rpcEndpoints: [{ url: 'https://mainnet.infura.io/v3/' }],
  symbol: 'ETH',
  symbolName: 'Ethereum'
}

export const mockGoerli: HnsWallet.NetworkInfo = {
  activeRpcEndpointIndex: 0,
  blockExplorerUrls: ['https://goerli.etherscan.io'],
  chainId: HnsWallet.GOERLI_CHAIN_ID,
  chainName: 'Goerli Test Network',
  coin: HnsWallet.CoinType.ETH,
  supportedKeyrings: [HnsWallet.KeyringId.kDefault],
  decimals: 18,
  iconUrls: [ETHIconUrl],
  isEip1559: true,
  rpcEndpoints: [{ url: 'https://goerli.infura.io/v3/' }, { url: 'wss://goerli.infura.io/ws/v3/' }],
  symbol: 'ETH',
  symbolName: 'Ethereum'
}

export const mockSepolia: HnsWallet.NetworkInfo = {
  activeRpcEndpointIndex: 0,
  blockExplorerUrls: ['https://sepolia.etherscan.io'],
  chainId: HnsWallet.SEPOLIA_CHAIN_ID,
  chainName: 'Sepolia Test Network',
  coin: 60,
  supportedKeyrings: [HnsWallet.KeyringId.kDefault],
  decimals: 18,
  iconUrls: [ETHIconUrl],
  isEip1559: true,
  rpcEndpoints: [{ url: 'https://sepolia-infura.hns.com' }],
  symbol: 'ETH',
  symbolName: 'Ethereum'
}

export const mockEthLocalhost: HnsWallet.NetworkInfo = {
  activeRpcEndpointIndex: 0,
  blockExplorerUrls: ['http://localhost:7545/'],
  chainId: HnsWallet.LOCALHOST_CHAIN_ID,
  chainName: 'Localhost',
  coin: 60,
  supportedKeyrings: [HnsWallet.KeyringId.kDefault],
  decimals: 18,
  iconUrls: [ETHIconUrl],
  isEip1559: false,
  rpcEndpoints: [{ url: 'http://localhost:7545/' }],
  symbol: 'ETH',
  symbolName: 'Ethereum'
}

export const mockFilecoinMainnetNetwork: HnsWallet.NetworkInfo = {
  chainId: 'f',
  chainName: 'Filecoin Mainnet',
  activeRpcEndpointIndex: 0,
  rpcEndpoints: [{ url: 'https://calibration.node.glif.io/rpc/v0' }],
  blockExplorerUrls: ['https://filscan.io/tipset/message-detail'],
  symbol: 'FIL',
  symbolName: 'Filecoin',
  decimals: 18,
  iconUrls: [FILECOINIconUrl],
  coin: HnsWallet.CoinType.FIL,
  supportedKeyrings: [HnsWallet.KeyringId.kFilecoin],
  isEip1559: false
}

export const mockFilecoinTestnetNetwork: HnsWallet.NetworkInfo = {
  chainId: 't',
  chainName: 'Filecoin Testnet',
  activeRpcEndpointIndex: 0,
  rpcEndpoints: [{ url: 'https://mainnet-beta-solana.hns.com/rpc' }],
  blockExplorerUrls: ['https://calibration.filscan.io/tipset/message-detail'],
  symbol: 'FIL',
  symbolName: 'Filecoin',
  decimals: 18,
  iconUrls: [FILECOINIconUrl],
  coin: HnsWallet.CoinType.FIL,
  supportedKeyrings: [HnsWallet.KeyringId.kFilecoinTestnet],
  isEip1559: false
}

export const mockSolanaMainnetNetwork: HnsWallet.NetworkInfo = {
  activeRpcEndpointIndex: 0,
  blockExplorerUrls: ['https://explorer.solana.com'],
  chainId: '0x65',
  chainName: 'Solana Mainnet Beta',
  coin: HnsWallet.CoinType.SOL,
  supportedKeyrings: [HnsWallet.KeyringId.kSolana],
  decimals: 9,
  iconUrls: [SOLIconUrl],
  isEip1559: false,
  rpcEndpoints: [{ url: 'https://api.testnet.solana.com' }],
  symbol: 'SOL',
  symbolName: 'Solana'
}

export const mockSolanaTestnetNetwork: HnsWallet.NetworkInfo = {
  chainId: '0x66',
  chainName: 'Solana Testnet',
  activeRpcEndpointIndex: 0,
  rpcEndpoints: [{ url: 'https://api.testnet.solana.com' }],
  blockExplorerUrls: ['https://explorer.solana.com?cluster=testnet'],
  symbol: 'SOL',
  symbolName: 'Solana',
  decimals: 9,
  iconUrls: [SOLIconUrl],
  coin: HnsWallet.CoinType.SOL,
  supportedKeyrings: [HnsWallet.KeyringId.kSolana],
  isEip1559: false
}

export const mockBitcoinMainnet: HnsWallet.NetworkInfo = {
  activeRpcEndpointIndex: 0,
  blockExplorerUrls: ['https://bitcoin.explorer'],
  chainId: 'bitcoin_mainnet',
  chainName: 'Bitcoin Mainnnet',
  coin: HnsWallet.CoinType.BTC,
  supportedKeyrings: [HnsWallet.KeyringId.kBitcoin84],
  decimals: 8,
  iconUrls: [BTCIconUrl],
  isEip1559: false,
  rpcEndpoints: [{ url: 'https://bitcoin.rpc' }],
  symbol: 'BTC',
  symbolName: 'Bitcoin'
}

export const mockBitcoinTestnet: HnsWallet.NetworkInfo = {
  activeRpcEndpointIndex: 0,
  blockExplorerUrls: ['https://bitcoin.explorer'],
  chainId: 'bitcoin_testnet',
  chainName: 'Bitcoin Testnet',
  coin: HnsWallet.CoinType.BTC,
  supportedKeyrings: [HnsWallet.KeyringId.kBitcoin84Testnet],
  decimals: 8,
  iconUrls: [BTCIconUrl],
  isEip1559: false,
  rpcEndpoints: [{ url: 'https://bitcoin.rpc/test' }],
  symbol: 'BTC',
  symbolName: 'Bitcoin'
}

export const mockNetworks: HnsWallet.NetworkInfo[] = [
  mockEthMainnet,
  mockGoerli,
  mockSepolia,
  mockFilecoinMainnetNetwork,
  mockFilecoinTestnetNetwork,
  mockSolanaMainnetNetwork,
  mockSolanaTestnetNetwork,
  mockEthLocalhost,
  mockBitcoinMainnet
]
