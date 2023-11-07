/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

import { defaultState, replaceStackWidgets } from '../../../hns_new_tab_ui/storage/new_tab_storage'

describe('new tab storage', () => {
  describe('replaceStackWidgets', () => {
    it('adds back widgets that should be showing in the stack', () => {
      const initialState = {
        ...defaultState,
        showRewards: true,
        hnsRewardsSupported: true,
        widgetStackOrder: []
      }
      const expectedState = {
        ...initialState,
        widgetStackOrder: ['rewards']
      }
      expect(replaceStackWidgets(initialState)).toEqual(expectedState)
    })
  })
})
