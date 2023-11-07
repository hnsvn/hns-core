// Copyright (c) 2023 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

import '//resources/cr_elements/md_select.css.js'
import {PolymerElement} from 'chrome://resources/polymer/v3_0/polymer/polymer_bundled.min.js';
import {WebUiListenerMixin} from 'chrome://resources/cr_elements/web_ui_listener_mixin.js';
import {PrefsMixin} from 'chrome://resources/cr_components/settings_prefs/prefs_mixin.js';
import {I18nMixin} from 'chrome://resources/cr_elements/i18n_mixin.js';
import {getTemplate} from './hns_leo_assistant_page.html.js'
import {HnsLeoAssistantBrowserProxy, HnsLeoAssistantBrowserProxyImpl}
  from './hns_leo_assistant_browser_proxy.js'


const HnsLeoAssistantPageBase =
  WebUiListenerMixin(I18nMixin(PrefsMixin(PolymerElement)))

/**
 * 'settings-hns-leo-assistant-page' is the settings page containing
 * hns's Leo Assistant features.
 */
class HnsLeoAssistantPageElement extends HnsLeoAssistantPageBase {
    static get is() {
        return 'settings-hns-leo-assistant-page'
    }

    static get template() {
        return getTemplate()
    }

    static get properties() {
      return {
        leoAssistantShowOnToolbarPref_: {
          type: Boolean,
          value: false,
          notify: true,
        },
      }
    }

    leoAssistantShowOnToolbarPref_: boolean
    browserProxy_: HnsLeoAssistantBrowserProxy =
      HnsLeoAssistantBrowserProxyImpl.getInstance()

    onResetAssistantData_() {
      this.browserProxy_.resetLeoData()
    }

    override ready () {
      super.ready()

      this.updateShowLeoAssistantIcon_()

      this.addWebUiListener('settings-hns-leo-assistant-changed',
      (isLeoVisible: boolean) => {
        this.leoAssistantShowOnToolbarPref_ = isLeoVisible
      })
    }

    itemPref_(enabled: boolean) {
      return {
        key: '',
        type: chrome.settingsPrivate.PrefType.BOOLEAN,
        value: enabled,
      }
    }

    private updateShowLeoAssistantIcon_() {
      this.browserProxy_.getLeoIconVisibility().then((result) => {
        this.leoAssistantShowOnToolbarPref_ = result
      })
    }

    onLeoAssistantShowOnToolbarChange_(e: any) {
      e.stopPropagation()
      this.browserProxy_.toggleLeoIcon()
    }
}

customElements.define(
  HnsLeoAssistantPageElement.is, HnsLeoAssistantPageElement)
