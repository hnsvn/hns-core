/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

import * as React from 'react'
import { shallow } from 'enzyme'
import { applicationState, torrentState, torrentObj } from '../testData'
import { TorrentState } from '../../../hns_webtorrent/extension/constants/webtorrentState'
import { HnsWebTorrentPage, mapStateToProps } from '../../../hns_webtorrent/extension/components/app'
import TorrentViewer from '../../../hns_webtorrent/extension/components/torrentViewer'
import MediaViewer from '../../../hns_webtorrent/extension/components/mediaViewer'

describe('HnsWebtorrentPage component', () => {
  describe('mapStateToProps', () => {
    it('should map the default state', () => {
      expect(mapStateToProps(applicationState, { tabId: 0 })).toEqual({
        torrentState
      })
    })
  })

  describe('render HnsWebtorrentPage component', () => {
    it('renders the MediaViewer component with a torrent and ix', () => {
      const torrentStateWithIx: TorrentState = { ...torrentState, ix: 1 }
      const wrapper = shallow(
        <HnsWebTorrentPage
          torrentState={torrentStateWithIx}
          torrentObj={torrentObj}
          actions={{}}
        />
      )

      const assertion = wrapper.find(MediaViewer)
      expect(assertion.length).toBe(1)
    })
    it('renders the TorrentViewer component with a valid torrent state', () => {
      const wrapper = shallow(
        <HnsWebTorrentPage
          torrentState={torrentState}
          actions={{}}
        />
      )

      expect(wrapper.find(TorrentViewer)).toHaveLength(1)
    })
  })
})
