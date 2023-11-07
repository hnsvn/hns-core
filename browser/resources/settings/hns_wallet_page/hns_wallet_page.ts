// Copyright (c) 2022 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

// @ts-nocheck TODO(petemill): Define types and remove ts-nocheck

import 'chrome://resources/cr_elements/cr_input/cr_input.js';
import './wallet_networks_subpage.js';

import {PrefsMixin} from 'chrome://resources/cr_components/settings_prefs/prefs_mixin.js';
import {I18nMixin} from 'chrome://resources/cr_elements/i18n_mixin.js';
import {WebUiListenerMixin} from 'chrome://resources/cr_elements/web_ui_listener_mixin.js';
import {PolymerElement} from 'chrome://resources/polymer/v3_0/polymer/polymer_bundled.min.js';

import {RouteObserverMixin, Router} from '../router.js';

import {HnsWalletBrowserProxy, HnsWalletBrowserProxyImpl} from './hns_wallet_browser_proxy.js';
import {getTemplate} from './hns_wallet_page.html.js'

const SettingsHnsWalletPageBase =
  WebUiListenerMixin(PrefsMixin(I18nMixin(RouteObserverMixin(PolymerElement))))

/**
 * 'settings-hns-default-extensions-page' is the settings page containing
 * hns's default extensions.
 */
class SettingsHnsWalletPage extends SettingsHnsWalletPageBase {
  static get is() {
    return 'settings-hns-wallet-page'
  }

  static get template() {
    return getTemplate()
  }

  static get properties() {
    return {
      isNativeWalletEnabled_: {
        type: Boolean
      },

      isNftPinningEnabled_: {
        type: Boolean
      },

      isNetworkEditor_: {
        type: Number,
        value: false,
      },

      shouldShowClearNftButton_: {
        type: Boolean,
        value: false,
      },

      shouldEnableClearNftButton_: {
        type: Boolean,
        value: false,
      },

      pinnedNftCount_: {
        type: Number,
        value: 0,
      },
    }
  }

  static get observers(){
    return [
      'onShowOptionChanged_(prefs.hns.wallet.auto_pin_enabled.value)'
    ]
  }

  browserProxy_ = HnsWalletBrowserProxyImpl.getInstance()

  ready() {
    super.ready()
    this.browserProxy_.getWeb3ProviderList().then(list => {
      // TODO(petemill): provide wallets type
      this.ethereum_provider_options_ = JSON.parse(list)
    });
    this.browserProxy_.getSolanaProviderOptions().then(list => {
      // TODO(petemill): provide wallets type
      this.solana_provider_options_ = list
    });
    this.browserProxy_.isNativeWalletEnabled().then(val => {
      this.isNativeWalletEnabled_ = val
    });
    this.browserProxy_.getAutoLockMinutes().then(val => {
      this.$.walletAutoLockMinutes.value = val
    })
    this.browserProxy_.isNftPinningEnabled().then(val => {
      this.isNftPinningEnabled_ = val
    });

    this.cryptocurrency_list_ = [
      { value: "BTC" },
      { value: "ETH" },
      { value: "LTC" },
      { value: "BCH" },
      { value: "BNB" },
      { value: "EOS" },
      { value: "XRP" },
      { value: "XLM" },
      { value: "LINK" },
      { value: "DOT" },
      { value: "YFI" }
    ]
    this.cryptocurrency_list_.every((x) => x.name = x.value);

    this.currency_list_ = [
      { value: 'AED' },
      { value: 'ARS' },
      { value: 'AUD' },
      { value: 'BDT' },
      { value: 'BHD' },
      { value: 'BMD' },
      { value: 'BRL' },
      { value: 'CAD' },
      { value: 'CHF' },
      { value: 'CLP' },
      { value: 'CZK' },
      { value: 'DKK' },
      { value: 'EUR' },
      { value: 'GBP' },
      { value: 'HKD' },
      { value: 'HUF' },
      { value: 'IDR' },
      { value: 'ILS' },
      { value: 'INR' },
      { value: 'JPY' },
      { value: 'KRW' },
      { value: 'KWD' },
      { value: 'LKR' },
      { value: 'MMK' },
      { value: 'MXN' },
      { value: 'MYR' },
      { value: 'NGN' },
      { value: 'NOK' },
      { value: 'NZD' },
      { value: 'PHP' },
      { value: 'PKR' },
      { value: 'PLN' },
      { value: 'RUB' },
      { value: 'SAR' },
      { value: 'SEK' },
      { value: 'SGD' },
      { value: 'THB' },
      { value: 'TRY' },
      { value: 'TWD' },
      { value: 'UAH' },
      { value: 'USD' },
      { value: 'VEF' },
      { value: 'VND' },
      { value: 'ZAR' },
      { value: 'XAG' },
      { value: 'XAU' },
      { value: 'XDR' }
    ]
    this.currency_list_.every((x) => x.name = x.value);
  }

  private onShowOptionChanged_() {
    this.shouldShowClearNftButton_ =
        !this.getPref('hns.wallet.auto_pin_enabled').value;

    this.browserProxy_.getPinnedNftCount().then(val => {
      this.pinnedNftCount_ = val
      this.shouldEnableClearNftButton_ = this.pinnedNftCount_ > 0
    })
  }

  onHnsWalletEnabledChange_() {
    this.browserProxy_.setHnsWalletEnabled(this.$.hnsWalletEnabled.checked);
  }

  isNetworkEditorRoute() {
    const router = Router.getInstance();
    return (router.getCurrentRoute() == router.getRoutes().HNS_WALLET_NETWORKS);
  }

  getPinnedNftCount() {
    return this.pinnedNftCount_;
  }

  /** @protected */
  currentRouteChanged() {
    this.isNetworkEditor_ = this.isNetworkEditorRoute()
  }

  onInputAutoLockMinutes_() {
    let value = Number(this.$.walletAutoLockMinutes.value)
    if (Number.isNaN(value) || value < 1 || value > 10080) {
      return
    }
    this.setPrefValue('hns.wallet.auto_lock_minutes', value)
  }

  onWalletNetworksEditorClick_() {
    const router = Router.getInstance();
    router.navigateTo(router.getRoutes().HNS_WALLET_NETWORKS);
  }

  onResetWallet_() {
    var message = this.i18n('walletResetConfirmation')
    if (window.prompt(message) !== this.i18n('walletResetConfirmationPhrase'))
      return
    this.browserProxy_.resetWallet()
    window.alert(this.i18n('walletResetConfirmed'))
  }

  onResetTransactionInfo_() {
    var message = this.i18n('walletResetTransactionInfoConfirmation')
    if (window.prompt(message) !== this.i18n('walletResetConfirmationPhrase'))
      return
    this.browserProxy_.resetTransactionInfo()
    window.alert(this.i18n('walletResetTransactionInfoConfirmed'))
  }

  onClearPinnedNftTapped_() {
    if (this.pinnedNftCount_ == 0) {
      return
    }
    var message = this.i18n('walletClearPinnedNftConfirmation')
    if (window.prompt(message) !== this.i18n('walletResetConfirmationPhrase'))
      return
    this.browserProxy_.clearPinnedNft().then(val => {
      this.onShowOptionChanged_()
    })
  }

  onNftDiscoveryEnabledChange_() {
    this.browserProxy_.setNftDiscoveryEnabled(this.$.enableNftDiscovery.checked)
  }
}

customElements.define(
  SettingsHnsWalletPage.is, SettingsHnsWalletPage)
