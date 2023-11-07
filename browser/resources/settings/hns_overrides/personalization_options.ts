// Copyright (c) 2021 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at https://mozilla.org/MPL/2.0/.

import '../hns_privacy_page/hns_personalization_options.js'

import {RegisterPolymerTemplateModifications} from 'chrome://resources/hns/polymer_overriding.js'
import {getTrustedHTML} from 'chrome://resources/js/static_types.js'

RegisterPolymerTemplateModifications({
  'settings-personalization-options': (templateContent) => {
    const metricsReportingControl = templateContent.getElementById('metricsReportingControl')
    if (!metricsReportingControl) {
      console.error(`[Hns Settings Overrides] Couldn't find metricsReportingControl`)
    } else {
      metricsReportingControl.insertAdjacentHTML(
        'beforebegin',
        getTrustedHTML`
          <settings-hns-personalization-options prefs="{{prefs}}">
          </settings-hns-personalization-options>
        `)
    }

    // searchSugestToggle is moved to search engines section.
    const searchSuggestToggleTemplate = templateContent.querySelector('template[is="dom-if"][if="[[showSearchSuggestToggle_()]]"]')
    if (!searchSuggestToggleTemplate) {
      console.error('[Hns Settings Overrides] Could not find searchSuggestToggle template')
    } else {
      const searchSuggestToggle = searchSuggestToggleTemplate.content.getElementById('searchSuggestToggle')
      if (!searchSuggestToggle) {
        console.error('[Hns Settings Overrides] Could not find searchSuggestToggle id')
      } else {
        searchSuggestToggle.setAttribute('hidden', 'true')
      }
    }
  },
})
