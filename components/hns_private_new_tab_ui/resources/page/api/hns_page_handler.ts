/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

import * as HnsPrivateNewTab from 'gen/hns/components/hns_private_new_tab_ui/common/hns_private_new_tab.mojom.m.js'

// Provide access to all the generated types
export * from 'gen/hns/components/hns_private_new_tab_ui/common/hns_private_new_tab.mojom.m.js'

interface API {
  pageHandler: HnsPrivateNewTab.PageHandlerRemote
  callbackRouter: HnsPrivateNewTab.PrivateTabPageCallbackRouter
}

let apiInstance: API

class PageHandlerAPI implements API {
  pageHandler: HnsPrivateNewTab.PageHandlerRemote
  callbackRouter: HnsPrivateNewTab.PrivateTabPageCallbackRouter

  constructor () {
    this.pageHandler = HnsPrivateNewTab.PageHandler.getRemote()
    this.callbackRouter = new HnsPrivateNewTab.PrivateTabPageCallbackRouter()
    this.pageHandler.setClientPage(this.callbackRouter.$.bindNewPipeAndPassRemote())
  }
}

export default function getPageHandlerInstance () {
  if (!apiInstance) {
    apiInstance = new PageHandlerAPI()
  }
  return apiInstance
}
