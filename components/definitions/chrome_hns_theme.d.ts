// Copyright (c) 2022 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at https://mozilla.org/MPL/2.0/.

declare namespace chrome.hnsTheme {
  type ThemeType = 'Light' | 'Dark' | 'System'
  type ThemeItem = {name: ThemeType, index: number}
  type ThemeList = ThemeItem[] // For backwards compatibility, but can be removed
  type ThemeTypeCallback = (themeType: ThemeType) => unknown
  type ThemeListCallback = (themeList: string /* JSON of type ThemeItem[] */) => unknown
  const getHnsThemeType: (themeType: ThemeTypeCallback) => void
  const getHnsThemeList: (cb: ThemeListCallback) => void
  const setHnsThemeType: (themeType: ThemeType) => void
  const onHnsThemeTypeChanged: {
    addListener: (callback: ThemeTypeCallback) => void
  }
}
