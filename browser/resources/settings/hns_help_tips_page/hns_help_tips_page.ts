/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

import {PrefsMixin} from 'chrome://resources/cr_components/settings_prefs/prefs_mixin.js';
import {WebUiListenerMixin} from 'chrome://resources/cr_elements/web_ui_listener_mixin.js';
import {html, PolymerElement} from 'chrome://resources/polymer/v3_0/polymer/polymer_bundled.min.js';

import {getTemplate} from './hns_help_tips_page.html.js'

const SettingHnsHelpTipsPageElementBase =
  WebUiListenerMixin(PrefsMixin(PolymerElement))

class SettingsHnsHelpTipsPageElement extends SettingHnsHelpTipsPageElementBase {
  static get is() {
    return 'settings-hns-help-tips-page'
  }

  static get template() {
    return getTemplate()
  }
}

customElements.define(
  SettingsHnsHelpTipsPageElement.is, SettingsHnsHelpTipsPageElement)
