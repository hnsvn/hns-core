/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

// @ts-nocheck TODO(petemill): Define types and remove ts-nocheck

import {loadTimeData} from '../i18n_setup.js'

import '//resources/js/cr.js';
import {PolymerElement} from '//resources/polymer/v3_0/polymer/polymer_bundled.min.js';

import {I18nMixin} from 'chrome://resources/cr_elements/i18n_mixin.js'
import {PrefsMixin} from 'chrome://resources/cr_components/settings_prefs/prefs_mixin.js';
import {HnsRewardsBrowserProxyImpl} from './hns_rewards_browser_proxy.js'
import {getTemplate} from './hns_rewards_page.html.js'

const SettingsHnsRewardsPageBase = I18nMixin(PrefsMixin(PolymerElement))

/**
 * 'settings-hns-rewards-page' is the settings page containing settings for
 * Hns Rewards.
 */
class SettingsHnsRewardsPage extends SettingsHnsRewardsPageBase {
  static get is() {
    return 'settings-hns-rewards-page'
  }

  static get template () {
    return getTemplate()
  }

  static get properties () {
    return {
      isRewardsEnabled_: {
        type: Boolean,
        value: false
      },
    }
  }

  private isRewardsEnabled_: boolean
  private browserProxy_ = HnsRewardsBrowserProxyImpl.getInstance()

  override ready () {
    super.ready()
    this.browserProxy_.getRewardsEnabled().then((enabled) => {
      if (enabled) {
        this.onRewardsEnabled_()
      }
    })
    chrome.hnsRewards.onRewardsWalletCreated.addListener(() => {
      // If Rewards hasn't been enabled before now, we know it is now so trigger
      // its handler
      if (!this.isRewardsEnabled_) {
        this.onRewardsEnabled_()
      }
    })
  }

  private openRewardsPanel_ () {
    chrome.hnsRewards.openRewardsPanel()
  }

  private openRewardsPage_ () {
    window.open('chrome://rewards', '_self')
  }

  private onRewardsEnabled_ () {
    this.isRewardsEnabled_ = true
  }

  private shouldShowRestartButtonForTipButtons_(
    buttonsEnabled: boolean,
    redditEnabled: boolean,
    twitterEnabled: boolean,
    githubEnabled: boolean) {
    return (
      buttonsEnabled !== this.browserProxy_.wasInlineTipButtonsEnabledAtStartup() ||
      redditEnabled !== this.browserProxy_.wasInlineTipRedditEnabledAtStartup() ||
      twitterEnabled !== this.browserProxy_.wasInlineTipTwitterEnabledAtStartup() ||
      githubEnabled !== this.browserProxy_.wasInlineTipGithubEnabledAtStartup())
  }

  private restartBrowser_ (e: Event) {
    e.stopPropagation()
    window.open('chrome://restart', '_self')
  }
}

customElements.define(
  SettingsHnsRewardsPage.is, SettingsHnsRewardsPage)
