/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

import * as cosmeticFilterEvents from '../../../../hns_extension/extension/hns_extension/background/events/cosmeticFilterEvents'

let selectorToReturn: string

global.prompt = () => {
  return selectorToReturn
}

describe('cosmeticFilterEvents events', () => {
  describe('when runtime.onMessage is received', () => {
    describe('contextMenuOpened', () => {
      it('assigns the base URI', () => {
        chrome.runtime.sendMessage({ type: 'contextMenuOpened', baseURI: 'hns.com' },
        () => {
          expect(cosmeticFilterEvents.rule.host).toBe('hns.com')
        })
      })
    })
  })

  describe('chrome.contextMenus.onClicked listener', () => {
    let contextMenuOnClickedSpy: jest.SpyInstance
    let chromeTabsQuerySpy: jest.SpyInstance
    let chromeTabsSendMessageSpy: jest.SpyInstance
    // eslint-disable-next-line @typescript-eslint/no-unused-vars
    let chromeHnsShieldsAddSiteCosmeticFilterSpy: jest.SpyInstance
    chrome.hnsShields = {
      addSiteCosmeticFilter: () => { /* stub */ }
    }
    beforeEach(() => {
      contextMenuOnClickedSpy = jest.spyOn(chrome.tabs, 'create')
      chromeTabsQuerySpy = jest.spyOn(chrome.tabs, 'query')
      chromeTabsSendMessageSpy = jest.spyOn(chrome.tabs, 'sendMessage')
      chromeHnsShieldsAddSiteCosmeticFilterSpy = jest.spyOn(chrome.hnsShields, 'addSiteCosmeticFilter')
    })
    afterEach(() => {
      contextMenuOnClickedSpy.mockRestore()
      chromeTabsSendMessageSpy.mockRestore()
    })

    describe('addBlockElement', function () {
      it('triggers addBlockElement action (query call)', function () {
        const info: chrome.contextMenus.OnClickData = { menuItemId: 'elementPickerMode', editable: false, pageUrl: 'hns.com' }
        // calls query
        const tab: chrome.tabs.Tab = {
          id: 3,
          index: 0,
          pinned: false,
          highlighted: false,
          windowId: 1,
          active: true,
          incognito: false,
          selected: true,
          discarded: false,
          autoDiscardable: false
        }
        cosmeticFilterEvents.onContextMenuClicked(info, tab)
        expect(chromeTabsQuerySpy).toBeCalled()
      })
    })
    describe('onSelectorReturned', function () {
      describe('after selector prompt is shown', function () {
        let insertCssSpy: jest.SpyInstance
        beforeEach(() => {
          insertCssSpy = jest.spyOn(chrome.tabs, 'insertCSS')
        })
        afterEach(() => {
          insertCssSpy.mockRestore()
        })
        it('calls `chrome.tabs.insertCSS` with cosmetic filter rule', function () {
          selectorToReturn = '#test_selector'
          cosmeticFilterEvents.applyCosmeticFilter('hns.com', selectorToReturn)
          let returnObj = {
            'code': '#test_selector {display: none !important;}',
            'cssOrigin': 'user'
          }
          expect(insertCssSpy).toBeCalledWith(returnObj, expect.any(Function))
        })
      })
    })
  })
})
