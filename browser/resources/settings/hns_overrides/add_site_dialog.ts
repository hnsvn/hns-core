// Copyright (c) 2022 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

import {RegisterPolymerComponentReplacement, RegisterPolymerTemplateModifications} from 'chrome://resources/hns/polymer_overriding.js'

import {HnsAddSiteDialogElement} from '../hns_add_site_dialog/hns_add_site_dialog.js'

RegisterPolymerComponentReplacement(
  'add-site-dialog',
  HnsAddSiteDialogElement
)


RegisterPolymerTemplateModifications({
  'add-site-dialog': (templateContent) => {
    const site = templateContent.querySelector('#site')
    if (!site) {
      console.error(
        `[Hns Settings Overrides] cannot find #site in add-site-dialog`)
      return
    }
    site.setAttribute('placeholder', '[[sitePlaceholder]]')
  }
})
