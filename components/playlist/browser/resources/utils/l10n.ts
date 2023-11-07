// Copyright (c) 2023 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

import { getLocale } from '$web-common/locale'

export type Message =
  | 'hnsDefaultPlaylistName'
  | 'hnsPlaylistContextMenuEdit'
  | 'hnsPlaylistContextMenuShare'
  | 'hnsPlaylistContextMenuKeepForOfflinePlaying'
  | 'hnsPlaylistContextMenuRemovePlayedContents'
  | 'hnsPlaylistContextMenuMove'
  | 'hnsPlaylistContextMenuRemoveOfflineData'
  | 'hnsPlaylistContextMenuRemoveFromPlaylist'
  | 'hnsPlaylistContextMenuRenamePlaylist'
  | 'hnsPlaylistContextMenuDeletePlaylist'

export function getLocalizedString (message: Message) {
  return getLocale(message)
}
