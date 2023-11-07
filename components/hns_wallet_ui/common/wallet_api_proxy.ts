// Copyright (c) 2021 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at https://mozilla.org/MPL/2.0/.

import { HnsWallet } from '../constants/types'

export class WalletApiProxy {
  walletHandler = new HnsWallet.WalletHandlerRemote()
  jsonRpcService = new HnsWallet.JsonRpcServiceRemote()
  bitcoinWalletService = new HnsWallet.BitcoinWalletServiceRemote()
  swapService = new HnsWallet.SwapServiceRemote()
  simulationService = new HnsWallet.SimulationServiceRemote()
  assetRatioService = new HnsWallet.AssetRatioServiceRemote()
  keyringService = new HnsWallet.KeyringServiceRemote()
  blockchainRegistry = new HnsWallet.BlockchainRegistryRemote()
  txService = new HnsWallet.TxServiceRemote()
  ethTxManagerProxy = new HnsWallet.EthTxManagerProxyRemote()
  solanaTxManagerProxy = new HnsWallet.SolanaTxManagerProxyRemote()
  filTxManagerProxy = new HnsWallet.FilTxManagerProxyRemote()
  hnsWalletService = new HnsWallet.HnsWalletServiceRemote()
  hnsWalletP3A = new HnsWallet.HnsWalletP3ARemote()
  hnsWalletPinService = new HnsWallet.WalletPinServiceRemote()
  hnsWalletAutoPinService = new HnsWallet.WalletAutoPinServiceRemote()
  hnsWalletIpfsService = new HnsWallet.IpfsServiceRemote()

  addJsonRpcServiceObserver (observer: HnsWallet.JsonRpcServiceObserverReceiver) {
    this.jsonRpcService.addObserver(observer.$.bindNewPipeAndPassRemote())
  }

  addKeyringServiceObserver (observer: HnsWallet.KeyringServiceObserverReceiver) {
    this.keyringService.addObserver(observer.$.bindNewPipeAndPassRemote())
  }

  addTxServiceObserver (observer: HnsWallet.TxServiceObserverReceiver) {
    this.txService.addObserver(observer.$.bindNewPipeAndPassRemote())
  }

  addHnsWalletServiceObserver (observer: HnsWallet.HnsWalletServiceObserverReceiver) {
    this.hnsWalletService.addObserver(observer.$.bindNewPipeAndPassRemote())
  }

  addHnsWalletServiceTokenObserver (observer: HnsWallet.HnsWalletServiceTokenObserverReceiver) {
    this.hnsWalletService.addTokenObserver(observer.$.bindNewPipeAndPassRemote())
  }

  addHnsWalletPinServiceObserver (observer: HnsWallet.HnsWalletPinServiceObserverReceiver) {
    this.hnsWalletPinService.addObserver(observer.$.bindNewPipeAndPassRemote())
  }

  addHnsWalletAutoPinServiceObserver (observer: HnsWallet.WalletAutoPinServiceObserverReceiver) {
    this.hnsWalletAutoPinService.addObserver(observer.$.bindNewPipeAndPassRemote())
  }
}

export default WalletApiProxy
