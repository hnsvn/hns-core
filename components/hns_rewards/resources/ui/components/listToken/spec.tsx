// Copyright (c) 2018 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at https://mozilla.org/MPL/2.0/.
import * as React from 'react'
import { shallow } from 'enzyme'
import { create } from 'react-test-renderer'
import ListToken from './index'
import { TestThemeProvider } from 'hns-ui/theme'

describe('ListToken tests', () => {
  const baseComponent = (props?: object) => <TestThemeProvider><ListToken id='list' {...props} value={10} /></TestThemeProvider>

  describe('basic tests', () => {
    it('matches the snapshot', () => {
      const component = baseComponent()
      const tree = create(component).toJSON()
      expect(tree).toMatchSnapshot()
    })

    it('renders the component', () => {
      const wrapper = shallow(baseComponent())
      const assertion = wrapper.find('#list').length
      expect(assertion).toBe(1)
    })
  })
})
