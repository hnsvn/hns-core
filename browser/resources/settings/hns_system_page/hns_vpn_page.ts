// Copyright (c) 2023 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

import 'chrome://resources/cr_components/settings_prefs/prefs.js';
import '../relaunch_confirmation_dialog.js';
import 'chrome://resources/cr_elements/cr_button/cr_button.js';
import 'chrome://resources/cr_elements/cr_icon_button/cr_icon_button.js';

import {PolymerElement} from 'chrome://resources/polymer/v3_0/polymer/polymer_bundled.min.js'
import {PrefsMixin, PrefsMixinInterface} from 'chrome://resources/cr_components/settings_prefs/prefs_mixin.js';
import {BaseMixin} from '../base_mixin.js'
import {loadTimeData} from "../i18n_setup.js"
import {getTemplate} from './hns_vpn_page.html.js'
import {RelaunchMixin, RelaunchMixinInterface, RestartType} from '../relaunch_mixin.js'
import {WebUiListenerMixin, WebUiListenerMixinInterface} from 'chrome://resources/cr_elements/web_ui_listener_mixin.js';
import {HnsVPNBrowserProxy, HnsVPNBrowserProxyImpl} from './hns_vpn_browser_proxy.js';
import {I18nMixin, I18nMixinInterface} from 'chrome://resources/cr_elements/i18n_mixin.js'
/**
 * 'settings-hns-vpn-page' is the settings page containing
 * hns's vpn features.
 */
const SettingsHnsVpnPageElementBase =
  PrefsMixin(BaseMixin(I18nMixin(WebUiListenerMixin(RelaunchMixin(
    PolymerElement))))) as {
    new(): PolymerElement &
           PrefsMixinInterface &
           RelaunchMixinInterface &
           WebUiListenerMixinInterface &
           I18nMixinInterface
  }

export class SettingsHnsVpnPageElement
    extends SettingsHnsVpnPageElementBase {

  static get is() {
    return 'settings-hns-vpn-page'
  }

  static get template() {
    return getTemplate()
  }

  static get properties() {
    return  {
      prefs: {
        type: Object,
        notify: true,
      },
      toggleWireguardSubLabel_: String,
      shouldShowRestart_: Boolean,
    }
  }

  private initialProtocolValue_: Boolean;
  private toggleWireguardSubLabel_: String;
  private hnsVpnConnected_: Boolean = false;
  private shouldShowRestart_: Boolean = false;

  private vpnBrowserProxy_: HnsVPNBrowserProxy =
    HnsVPNBrowserProxyImpl.getInstance();

  override ready() {
    super.ready();
    this.initialProtocolValue_ = this.getCurrentPrefValue()
    this.updateState()
    this.addWebUiListener('hns-vpn-state-change', this.onVpnStateChange.bind(this))
    // <if expr="is_win">
    this.vpnBrowserProxy_.isHnsVpnConnected().then(this.onVpnStateChange.bind(this))
    // </if>
  }

  private onVpnStateChange(connected: boolean) {
    this.hnsVpnConnected_ = connected
    if (this.hnsVpnConnected_) {
      this.resetToInitialValue()
    }
    this.updateState()
  }

  private getCurrentPrefValue(): boolean {
    return this.getPref('hns.hns_vpn.wireguard_enabled').value
  }

  private updateState() {
    this.toggleWireguardSubLabel_ = this.hnsVpnConnected_ ?
      this.i18n('sublabelVpnConnected') : this.i18n('sublabelVpnDisconnected')
    this.shouldShowRestart_ =
        (this.initialProtocolValue_ !== this.getCurrentPrefValue()) &&
        !this.hnsVpnConnected_;
  }

  private resetToInitialValue() {
    this.setPrefValue('hns.hns_vpn.wireguard_enabled',
      this.initialProtocolValue_)
  }

  private showVpnPage_(): boolean {
    return loadTimeData.getBoolean('isHnsVPNEnabled')
  }

  private isWireguardServiceRegistered(success: boolean) {
    if (success)
      return;
    // Try to register it.
    this.vpnBrowserProxy_.registerWireguardService().then(
        (success: boolean) => {
      if (!success) {
        this.resetToInitialValue()
        this.updateState()
        return;
      }
    })
  }

  private onChange_() {
    this.updateState();
    if (!this.getCurrentPrefValue())
      return
    // <if expr="is_win">
    // If user enabled Wireguard service we have to check if it was registered.
    this.vpnBrowserProxy_.isWireguardServiceRegistered().then(
      this.isWireguardServiceRegistered.bind(this))
    // </if>
  }

  private onRestartClick_(e: Event) {
    // Prevent event from bubbling up to the toggle button.
    e.stopPropagation();
    this.performRestart(RestartType.RESTART);
  }
}

declare global {
  interface HTMLElementTagNameMap {
    'settings-hns-vpn-page': SettingsHnsVpnPageElement
  }
}

customElements.define(
  SettingsHnsVpnPageElement.is, SettingsHnsVpnPageElement)
