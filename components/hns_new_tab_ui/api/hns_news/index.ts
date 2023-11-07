// Copyright (c) 2021 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

import * as HnsNews from 'gen/hns/components/hns_news/common/hns_news.mojom.m.js'
// Provide access to all the generated types
export * from 'gen/hns/components/hns_news/common/hns_news.mojom.m.js'

// Provide easy access to types which mojom functions return but aren't
// defined as a struct.
export type Publishers = Record<string, HnsNews.Publisher>
export type Channels = Record<string, HnsNews.Channel>

// Create singleton connection to browser interface
let hnsNewsControllerInstance: HnsNews.HnsNewsControllerRemote

export default function getHnsNewsController () {
  // Make connection on first call (not in module root, so that storybook
  // doesn't try to connect, or pages which use exported types
  // but ultimately don't fetch any data.
  if (!hnsNewsControllerInstance) {
    // In Storybook, we have a mocked HnsNewsController because none of the
    // mojo apis are available.
    // @ts-expect-error
    hnsNewsControllerInstance = window.storybookHnsNewsController || HnsNews.HnsNewsController.getRemote()
  }
  return hnsNewsControllerInstance
}

export const isPublisherEnabled = (publisher: HnsNews.Publisher) => {
  if (!publisher) return false

  // Direct Sources are enabled if they're available.
  if (publisher.type === HnsNews.PublisherType.DIRECT_SOURCE) return true

  // Publishers enabled via channel are not shown in the sidebar.
  return publisher.userEnabledStatus === HnsNews.UserEnabled.ENABLED
}

export const isDirectFeed = (publisher: HnsNews.Publisher) => {
  if (!publisher) return false
  return publisher.type === HnsNews.PublisherType.DIRECT_SOURCE
}
