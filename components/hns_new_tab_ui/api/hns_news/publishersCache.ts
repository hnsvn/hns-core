// Copyright (c) 2022 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

import {
  HnsNewsControllerRemote,
  Publisher,
  PublishersListenerInterface,
  PublishersListenerReceiver,
  UserEnabled
} from 'gen/hns/components/hns_news/common/hns_news.mojom.m'
import getHnsNewsController, { isDirectFeed } from '.'
import { CachingWrapper } from '$web-common/mojomCache'

export class PublishersCachingWrapper
  extends CachingWrapper<Publisher>
  implements PublishersListenerInterface {
  private receiver = new PublishersListenerReceiver(this)
  private controller: HnsNewsControllerRemote

  constructor () {
    super()

    this.controller = getHnsNewsController()

    // We can't set up  the mojo pipe in the test environment.
    if (process.env.NODE_ENV !== 'test') {
      this.controller.addPublishersListener(
        this.receiver.$.bindNewPipeAndPassRemote()
      )
    }
  }

  setPublisherFollowed (publisherId: string, enabled: boolean) {
    const copy = {
      ...this.cache
    }

    if (isDirectFeed(this.cache[publisherId]) && !enabled) {
      this.controller.setPublisherPref(publisherId, UserEnabled.DISABLED)
      delete copy[publisherId]
    } else {
      const status = enabled ? UserEnabled.ENABLED : UserEnabled.NOT_MODIFIED
      this.controller.setPublisherPref(publisherId, status)
      copy[publisherId].userEnabledStatus = status
    }

    this.change(copy)
  }
}
