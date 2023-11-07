// Copyright (c) 2021 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at https://mozilla.org/MPL/2.0/.

import * as HnsVPN from 'gen/hns/components/hns_vpn/common/mojom/hns_vpn.mojom.m.js'
// Provide access to all the generated types
export * from 'gen/hns/components/hns_vpn/common/mojom/hns_vpn.mojom.m.js'

export type SupportData = HnsVPN.ServiceHandler_GetSupportData_ResponseParams

interface API {
  pageCallbackRouter: HnsVPN.PageInterface
  panelHandler: HnsVPN.PanelHandlerInterface
  serviceHandler: HnsVPN.ServiceHandlerInterface
}

let panelBrowserAPIInstance: API
class PanelBrowserAPI implements API {
  pageCallbackRouter = new HnsVPN.PageCallbackRouter()
  panelHandler = new HnsVPN.PanelHandlerRemote()
  serviceHandler = new HnsVPN.ServiceHandlerRemote()

  constructor () {
    const factory = HnsVPN.PanelHandlerFactory.getRemote()
    factory.createPanelHandler(
      this.pageCallbackRouter.$.bindNewPipeAndPassRemote(),
      this.panelHandler.$.bindNewPipeAndPassReceiver(),
      this.serviceHandler.$.bindNewPipeAndPassReceiver()
    )
  }
}

export default function getPanelBrowserAPI () {
  if (!panelBrowserAPIInstance) {
    panelBrowserAPIInstance = new PanelBrowserAPI()
  }
  return panelBrowserAPIInstance
}

export function setPanelBrowserApiForTesting (api: API) {
  panelBrowserAPIInstance = api
}
