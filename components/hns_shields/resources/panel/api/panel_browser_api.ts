// Copyright (c) 2021 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at https://mozilla.org/MPL/2.0/.

import * as HnsShields from 'gen/hns/components/hns_shields/common/hns_shields_panel.mojom.m.js'
// Provide access to all the generated types
export * from 'gen/hns/components/hns_shields/common/hns_shields_panel.mojom.m.js'

interface API {
  panelHandler: HnsShields.PanelHandlerRemote
  dataHandler: HnsShields.DataHandlerRemote
}

let panelBrowserAPIInstance: API
class PanelBrowserAPI implements API {
  panelHandler = new HnsShields.PanelHandlerRemote()
  dataHandler = new HnsShields.DataHandlerRemote()

  constructor () {
    const factory = HnsShields.PanelHandlerFactory.getRemote()
    factory.createPanelHandler(
      this.panelHandler.$.bindNewPipeAndPassReceiver(),
      this.dataHandler.$.bindNewPipeAndPassReceiver()
    )
  }
}

export default function getPanelBrowserAPI () {
  if (!panelBrowserAPIInstance) {
    panelBrowserAPIInstance = new PanelBrowserAPI()
  }
  return panelBrowserAPIInstance
}