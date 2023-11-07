// Copyright (c) 2022 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

import './hns_extensions_manifest_v2_subpage.js';

import {PrefsMixin, PrefsMixinInterface} from 'chrome://resources/cr_components/settings_prefs/prefs_mixin.js';
import {WebUiListenerMixin, WebUiListenerMixinInterface} from 'chrome://resources/cr_elements/web_ui_listener_mixin.js'
import {PolymerElement} from 'chrome://resources/polymer/v3_0/polymer/polymer_bundled.min.js'

import {SettingsCheckboxElement} from '../controls/settings_checkbox.js';
import {loadTimeData} from '../i18n_setup.js';
import {RouteObserverMixin, Router} from '../router.js';

import {HnsDefaultExtensionsBrowserProxyImpl} from './hns_default_extensions_browser_proxy.js'
import {getTemplate} from './hns_default_extensions_page.html.js'

const SettingHnsDefaultExtensionsPageElementBase =
  WebUiListenerMixin(PrefsMixin(RouteObserverMixin(PolymerElement))) as {
  new (): PolymerElement & WebUiListenerMixinInterface & PrefsMixinInterface
}

export interface SettingHnsDefaultExtensionsPageElement {
  $: {
    widevineEnabled: SettingsCheckboxElement,
    webTorrentEnabled: SettingsCheckboxElement,
    hangoutsEnabled: SettingsCheckboxElement
  }
}

/**
 * 'settings-hns-default-extensions-page' is the settings page containing
 * hns's default extensions.
 */
export class SettingHnsDefaultExtensionsPageElement extends SettingHnsDefaultExtensionsPageElementBase {
  static get is() {
    return 'settings-hns-default-extensions-page'
  }

  static get template() {
    return getTemplate()
  }

  static get properties() {
    return {
      showRestartToast_: Boolean,
      widevineEnabledPref_: {
        type: Object,
        value() {
          // TODO(dbeam): this is basically only to appease PrefControlMixin.
          // Maybe add a no-validate attribute instead? This makes little sense.
          return {}
        },
      },
      isExtensionsManifestV2FeatureEnabled_: Boolean,
      isExtensionsManifestV2Routed_: {
        type: Boolean,
        value: false,
      },
    }
  }

  private browserProxy_ = HnsDefaultExtensionsBrowserProxyImpl.getInstance()
  showRestartToast_: boolean
  widevineEnabledPref_: chrome.settingsPrivate.PrefObject
  isExtensionsManifestV2FeatureEnabled_: boolean
  isExtensionsManifestV2Routed_: boolean

  override ready() {
    super.ready()

    this.isExtensionsManifestV2FeatureEnabled_ =
      loadTimeData.getBoolean('extensionsManifestV2Feature')

    this.addWebUiListener(
      'hns-needs-restart-changed', (needsRestart: boolean) => {
        this.showRestartToast_ = needsRestart
    })
    this.browserProxy_.getRestartNeeded().then(show => {
      this.showRestartToast_ = show
    })

    // PrefControlMixin checks for a pref being valid, so have to fake it,
    // same as upstream.
    const setWidevineEnabledPref = (enabled: boolean) => this.setWidevineEnabledPref_(enabled)
    this.addWebUiListener('widevine-enabled-changed', setWidevineEnabledPref)
    this.browserProxy_.isWidevineEnabled().then(setWidevineEnabledPref)
  }

  /** @protected */
  currentRouteChanged() {
    const router = Router.getInstance();
    this.isExtensionsManifestV2Routed_ =
      router.getCurrentRoute() == router.getRoutes().EXTENSIONS_V2;
  }

  onWebTorrentEnabledChange_() {
    this.browserProxy_.setWebTorrentEnabled(this.$.webTorrentEnabled.checked)
  }

  onHangoutsEnabledChange_() {
    this.browserProxy_.setHangoutsEnabled(this.$.hangoutsEnabled.checked)
  }

  restartBrowser_(e: Event) {
    e.stopPropagation()
    window.open("chrome://restart", "_self")
  }

  setWidevineEnabledPref_(enabled: boolean) {
    const pref = {
      key: '',
      type: chrome.settingsPrivate.PrefType.BOOLEAN,
      value: enabled,
    }
    this.widevineEnabledPref_ = pref
  }

  onWidevineEnabledChange_() {
    this.browserProxy_.setWidevineEnabled(this.$.widevineEnabled.checked)
  }

  openManageV2ExtensionPage_() {
    const router = Router.getInstance();
    router.navigateTo(router.getRoutes().EXTENSIONS_V2);
  }

  openExtensionsPage_() {
    window.open("chrome://extensions", "_self")
  }

  openKeyboardShortcutsPage_() {
    window.open("chrome://extensions/shortcuts", "_self")
  }

  openWebStoreUrl_() {
    window.open(loadTimeData.getString('getMoreExtensionsUrl'), undefined, 'noreferrer')
  }

  shouldShowRestartForGoogleLogin_(value: boolean) {
    return this.browserProxy_.wasSignInEnabledAtStartup() != value
  }

  shouldShowRestartForMediaRouter_(value: boolean) {
    return this.browserProxy_.isMediaRouterEnabled() != value
  }
}

customElements.define(
  SettingHnsDefaultExtensionsPageElement.is, SettingHnsDefaultExtensionsPageElement)
