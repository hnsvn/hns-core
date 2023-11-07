/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

import '../../../hns_new_tab_ui/stories/default/data/mockHnsNewsController'

import getActions from '../../../hns_new_tab_ui/api/getActions'
import { types as topSitesTypes } from '../../../hns_new_tab_ui/constants/grid_sites_types'

describe('new tab data api tests', () => {
  describe('getActions', () => {
    it('can call an action from getActions', () => {
      expect(getActions().showTilesRemovedNotice(true)).toEqual({
        payload: { shouldShow: true },
        type: topSitesTypes.SHOW_TILES_REMOVED_NOTICE
      })
    })
  })
})
