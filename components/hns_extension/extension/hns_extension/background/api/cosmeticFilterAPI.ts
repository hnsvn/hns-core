/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

export const addSiteCosmeticFilter = (host: string, cssSelector: string) => {
  chrome.hnsShields.addSiteCosmeticFilter(host, cssSelector)
}

export const openFilterManagementPage = () => {
  chrome.hnsShields.openFilterManagementPage()
}
