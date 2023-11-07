// Copyright (c) 2021 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at https://mozilla.org/MPL/2.0/.

import WalletApiProxy from '../common/wallet_api_proxy'
import { HnsWallet } from '../constants/types'

let walletPageApiProxyInstance: WalletPageApiProxy

export class WalletPageApiProxy extends WalletApiProxy {
  callbackRouter = new HnsWallet.PageCallbackRouter()
  pageHandler = new HnsWallet.PageHandlerRemote()
  constructor () {
    super()

    const factory = HnsWallet?.PageHandlerFactory?.getRemote?.()
    factory?.createPageHandler?.(
      this.callbackRouter.$.bindNewPipeAndPassRemote(),
      this.pageHandler.$.bindNewPipeAndPassReceiver(),
      this.walletHandler.$.bindNewPipeAndPassReceiver(),
      this.jsonRpcService.$.bindNewPipeAndPassReceiver(),
      this.bitcoinWalletService.$.bindNewPipeAndPassReceiver(),
      this.swapService.$.bindNewPipeAndPassReceiver(),
      this.assetRatioService.$.bindNewPipeAndPassReceiver(),
      this.keyringService.$.bindNewPipeAndPassReceiver(),
      this.blockchainRegistry.$.bindNewPipeAndPassReceiver(),
      this.txService.$.bindNewPipeAndPassReceiver(),
      this.ethTxManagerProxy.$.bindNewPipeAndPassReceiver(),
      this.solanaTxManagerProxy.$.bindNewPipeAndPassReceiver(),
      this.filTxManagerProxy.$.bindNewPipeAndPassReceiver(),
      this.hnsWalletService.$.bindNewPipeAndPassReceiver(),
      this.hnsWalletP3A.$.bindNewPipeAndPassReceiver(),
      this.hnsWalletPinService.$.bindNewPipeAndPassReceiver(),
      this.hnsWalletAutoPinService.$.bindNewPipeAndPassReceiver(),
      this.hnsWalletIpfsService.$.bindNewPipeAndPassReceiver())
  }
}

export default function getWalletPageApiProxy () {
  if (!walletPageApiProxyInstance) {
    walletPageApiProxyInstance = new WalletPageApiProxy()
  }
  return walletPageApiProxyInstance
}
