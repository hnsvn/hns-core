/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

import { types } from '../../../../hns_rewards/resources/page/actions/rewards_types'
import * as actions from '../../../../hns_rewards/resources/page/actions/rewards_actions'

describe('rewards_actions', () => {
  it('onSettingSave', () => {
    expect(actions.onSettingSave('key', 'value')).toEqual({
      type: types.ON_SETTING_SAVE,
      meta: undefined,
      payload: {
        key: 'key',
        persist: true,
        value: 'value'
      }
    })
  })
})
