// Copyright (c) 2020 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at https://mozilla.org/MPL/2.0/.

import {PolymerElement, html} from 'chrome://resources/polymer/v3_0/polymer/polymer_bundled.min.js'
import {WebUiListenerMixin, WebUiListenerMixinInterface} from 'chrome://resources/cr_elements/web_ui_listener_mixin.js'
import {routes} from '../route.js'
import {Router} from '../router.js'
import 'chrome://resources/cr_elements/md_select.css.js';
import '../settings_shared.css.js'
import '../settings_vars.css.js'
import {loadTimeData} from "../i18n_setup.js"
import {HnsAppearanceBrowserProxy,  HnsAppearanceBrowserProxyImpl} from './hns_appearance_browser_proxy.js'
import {BaseMixin} from '../base_mixin.js'
import {getTemplate} from './hns_theme.html.js'

export interface SettingsHnsAppearanceThemeElement {
  $: {
    hnsThemeType: HTMLSelectElement
  }
}

const SettingsHnsAppearanceThemeElementBase =
  WebUiListenerMixin(BaseMixin(PolymerElement)) as {
  new (): PolymerElement & WebUiListenerMixinInterface
}

/**
 * 'settings-hns-appearance-theme' is the settings page area containing
 * hns's appearance related settings that located at the top of appearance
 * area.
 */
export class SettingsHnsAppearanceThemeElement extends SettingsHnsAppearanceThemeElementBase {
  static get is() {
    return 'settings-hns-appearance-theme'
  }

  static get template() {
    return getTemplate()
  }

  static get observers() {
    return [
      'updateSelected_(hnsThemeType_, hnsThemeList_)',
    ]
  }

  browserProxy_: HnsAppearanceBrowserProxy = HnsAppearanceBrowserProxyImpl.getInstance()
  hnsThemeList_: chrome.hnsTheme.ThemeItem[]
  hnsThemeType_: number // index of current theme type in hnsThemeList_

  override ready() {
    super.ready()

    this.addWebUiListener('hns-theme-type-changed', (type: number) => {
      this.hnsThemeType_ = type;
    })
    this.browserProxy_.getHnsThemeList().then((list) => {
      this.hnsThemeList_ = JSON.parse(list) as chrome.hnsTheme.ThemeItem[];
    })
    this.browserProxy_.getHnsThemeType().then(type => {
      this.hnsThemeType_ = type;
    })
  }

  private onHnsThemeTypeChange_() {
    this.browserProxy_.setHnsThemeType(Number(this.$.hnsThemeType.value))
  }

  private hnsThemeTypeEqual_(theme1: string, theme2: string) {
    return theme1 === theme2
  }

  private onThemeTap_() {
    Router.getInstance().navigateTo(routes.THEMES)
  }

  private updateSelected_() {
    // Wait for the dom-repeat to populate the <select> before setting
    // <select>#value so the correct option gets selected.
    setTimeout(() => {
      this.$.hnsThemeType.value = String(this.hnsThemeType_)
    })
  }

  useThemesSubPage_() {
    return loadTimeData.valueExists('superReferralThemeName') &&
      loadTimeData.getString('superReferralThemeName') !== ''
  }
}

customElements.define(
    SettingsHnsAppearanceThemeElement.is, SettingsHnsAppearanceThemeElement)
