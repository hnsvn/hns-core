// Copyright (c) 2020 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

// @ts-nocheck TODO(petemill): Define types and remove ts-nocheck

import '../hns_default_extensions_page/hns_default_extensions_page.js'
import '../hns_help_tips_page/hns_help_tips_page.js'
import '../hns_ipfs_page/hns_ipfs_page.js'
import '../hns_new_tab_page/hns_new_tab_page.js'
import '../hns_rewards_page/hns_rewards_page.js'
import '../hns_search_engines_page/hns_search_engines_page.js'
import '../hns_sync_page/hns_sync_page.js'
import '../hns_site_settings/hns_site_data_details_subpage.js'
import '../hns_tor_page/hns_tor_subpage.js'
import '../hns_wallet_page/hns_wallet_page.js'
import '../hns_web3_domains_page/hns_web3_domains_page.js'
import '../default_hns_shields_page/default_hns_shields_page.js'
import '../getting_started_page/getting_started.js'
import '../social_blocking_page/social_blocking_page.js'
import '../hns_leo_assistant_page/hns_leo_assistant_page.js'

import {html, RegisterPolymerTemplateModifications, RegisterStyleOverride} from 'chrome://resources/hns/polymer_overriding.js'

import {loadTimeData} from '../i18n_setup.js'

const isGuest = loadTimeData.getBoolean('isGuest')

export function getSectionElement (templateContent, sectionName) {
  const sectionEl = templateContent.querySelector(`template[if*='showPage_(pageVisibility.${sectionName}']`) ||
    templateContent.querySelector(`template[if*='pageVisibility.${sectionName}']`) ||
    templateContent.querySelector(`settings-section[section="${sectionName}"]`)
  if (!sectionEl) {
    console.error(`[Hns Settings Overrides] Could not find section '${sectionName}'`)
  }
  return sectionEl
}

/**
 * Creates a settings-section element with a single child and returns it.
 * @param {string} sectionName - value of the section attribute
 * @param {string} titleName - loadTimeData key for page-title
 * @param {string} childName - name of child element
 * @param {Object} childAttributes - key-value pairs of child element attributes
 * @returns {Element}
 */
function createSectionElement (sectionName, titleName, childName, childAttributes) {
  const childAttributesString = Object.keys(childAttributes).map(attribute =>
      `${attribute}="${childAttributes[attribute]}"`)
    .join(' ')
  // This needs to be inside a template so that our components do not get created immediately.
  // Otherwise the polymer bindings won't be setup correctly at first.
  return html`
    <settings-section page-title="${loadTimeData.getString(titleName)}" section="${sectionName}">
      <${childName}
        ${childAttributesString}
      >
      </${childName}>
    </settings-section>
  `
}

function createNestedSectionElement(sectionName, nestedUnder, titleName, childName, childAttributes) {
  const childAttributesString = Object.keys(childAttributes).map(attribute =>
    `${attribute}="${childAttributes[attribute]}"`)
    .join(' ')
  // This needs to be inside a template so that our components do not get created immediately.
  // Otherwise the polymer bindings won't be setup correctly at first.
  return html`
    <settings-section page-title="${loadTimeData.getString(titleName)}" section="${sectionName}" nest-under-section="${nestedUnder}">
      <${childName}
        ${childAttributesString}
      >
      </${childName}>
    </settings-section>
  `
}

RegisterStyleOverride(
  'settings-basic-page',
  html`
    <style>
      :host {
        min-width: 544px !important;
      }
    </style>
  `
)

RegisterPolymerTemplateModifications({
  'settings-basic-page': (templateContent) => {
    // Add 'Getting Started' section
    // Entire content is wrapped in another conditional template
    const actualTemplate = templateContent.querySelector('template')
    if (!actualTemplate) {
      console.error('[Hns Settings Overrides] Could not find basic-page template')
      return
    }
    const basicPageEl = actualTemplate.content.querySelector('#basicPage')
    if (!basicPageEl) {
      console.error('[Hns Settings Overrides] Could not find basicPage element to insert Getting Started section')
    } else {
      const privacyGuidePromoSection = actualTemplate.content.querySelector('#privacyGuidePromoSection')
      if (!privacyGuidePromoSection) {
        console.error('[Hns Settings Overrides] Could not find privacyGuidePromoSection element to hide')
      } else {
        privacyGuidePromoSection.remove()
      }
      const sectionGetStarted = document.createElement('template')
      sectionGetStarted.setAttribute('is', 'dom-if')
      sectionGetStarted.setAttribute('restamp', true)
      sectionGetStarted.setAttribute('if', '[[showPage_(pageVisibility.getStarted)]]')
      sectionGetStarted.content.appendChild(createSectionElement(
        'getStarted',
        'hnsGetStartedTitle',
        'hns-settings-getting-started',
        {
          prefs: '{{prefs}}',
          'page-visibility': '[[pageVisibility]]'
        }
      ))
      const sectionExtensions = document.createElement('template')
      sectionExtensions.setAttribute('is', 'dom-if')
      sectionExtensions.setAttribute('restamp', true)
      sectionExtensions.setAttribute('if', '[[showPage_(pageVisibility.extensions)]]')
      sectionExtensions.content.appendChild(createSectionElement(
        'extensions',
        'hnsDefaultExtensions',
        'settings-hns-default-extensions-page',
        {
          prefs: '{{prefs}}'
        }
      ))
      const sectionIPFS = document.createElement('template')
      sectionIPFS.setAttribute('is', 'dom-if')
      sectionIPFS.setAttribute('restamp', true)
      sectionIPFS.setAttribute('if', '[[showPage_(pageVisibility.hnsIPFS)]]')
      sectionIPFS.content.appendChild(createNestedSectionElement(
        'ipfs',
        'web3',
        'hnsIPFS',
        'settings-hns-ipfs-page',
        {
          prefs: '{{prefs}}'
        }
      ))
      const sectionTor = document.createElement('template')
      sectionTor.setAttribute('is', 'dom-if')
      sectionTor.setAttribute('restamp', true)
      sectionTor.setAttribute('if', '[[showPage_(pageVisibility.hnsTor)]]')
      sectionTor.content.appendChild(createNestedSectionElement(
        'tor',
        'privacy',
        'hnsTor',
        'settings-hns-tor-subpage',
        {
          prefs: '{{prefs}}'
        }
      ))
      const isHnsWalletAllowed = loadTimeData.getBoolean('isHnsWalletAllowed')
      let sectionWallet = undefined
      if (isHnsWalletAllowed) {
        sectionWallet = document.createElement('template')
        sectionWallet.setAttribute('is', 'dom-if')
        sectionWallet.setAttribute('restamp', true)
        sectionWallet.setAttribute('if', '[[showPage_(pageVisibility.hnsWallet)]]')
        sectionWallet.content.appendChild(createNestedSectionElement(
          'wallet',
          'web3',
          'hnsWallet',
          'settings-hns-wallet-page',
          {
            prefs: '{{prefs}}'
          }
        ))
      }
      const sectionWeb3Domains = document.createElement('template')
      sectionWeb3Domains.setAttribute('is', 'dom-if')
      sectionWeb3Domains.setAttribute('restamp', true)
      sectionWeb3Domains.setAttribute('if',
        '[[showPage_(pageVisibility.hnsWeb3Domains)]]')
      sectionWeb3Domains.content.appendChild(createNestedSectionElement(
        'web3Domains',
        'web3',
        'hnsWeb3Domains',
        'settings-hns-web3-domains-page',
        {
          prefs: '{{prefs}}'
        }
      ))
      const sectionSync = document.createElement('template')
      sectionSync.setAttribute('is', 'dom-if')
      sectionSync.setAttribute('restamp', true)
      sectionSync.setAttribute('if', '[[showPage_(pageVisibility.hnsSync)]]')
      sectionSync.content.appendChild(createSectionElement(
        'hnsSync',
        'hnsSync',
        'settings-hns-sync-page',
        {}
      ))
      const sectionShields = document.createElement('template')
      sectionShields.setAttribute('is', 'dom-if')
      sectionShields.setAttribute('restamp', true)
      sectionShields.setAttribute('if', '[[showPage_(pageVisibility.shields)]]')
      sectionShields.content.appendChild(createSectionElement(
        'shields',
        'hnsShieldsTitle',
        'settings-default-hns-shields-page',
        {
          prefs: '{{prefs}}'
        }
      ))
      const isHnsRewardsSupported = loadTimeData.getBoolean('isHnsRewardsSupported')
      let sectionRewards = undefined
      if (isHnsRewardsSupported) {
        sectionRewards = document.createElement('template')
        sectionRewards.setAttribute('is', 'dom-if')
        sectionRewards.setAttribute('restamp', true)
        sectionRewards.setAttribute('if', '[[showPage_(pageVisibility.rewards)]]')
        sectionRewards.content.appendChild(createSectionElement(
          'rewards',
          'hnsRewards',
          'settings-hns-rewards-page',
          {
            prefs: '{{prefs}}'
          }
        ))
      }
      const sectionSocialBlocking = document.createElement('template')
      sectionSocialBlocking.setAttribute('is', 'dom-if')
      sectionSocialBlocking.setAttribute('restamp', true)
      sectionSocialBlocking.setAttribute('if', '[[showPage_(pageVisibility.socialBlocking)]]')
      sectionSocialBlocking.content.appendChild(createSectionElement(
        'socialBlocking',
        'socialBlocking',
        'settings-social-blocking-page',
        {
          prefs: '{{prefs}}'
        }
      ))
      const sectionHelpTips = document.createElement('template')
      sectionHelpTips.setAttribute('is', 'dom-if')
      sectionHelpTips.setAttribute('restamp', true)
      sectionHelpTips.setAttribute('if', '[[showPage_(pageVisibility.hnsHelpTips)]]')
      sectionHelpTips.content.appendChild(createSectionElement(
        'hnsHelpTips',
        'hnsHelpTips',
        'settings-hns-help-tips-page',
        {
          prefs: '{{prefs}}'
        }
      ))
      const sectionLeoAssist = document.createElement('template')
      sectionLeoAssist.setAttribute('is', 'dom-if')
      sectionLeoAssist.setAttribute('restamp', true)
      sectionLeoAssist
        .setAttribute('if', '[[showPage_(pageVisibility.leoAssistant)]]')
      sectionLeoAssist.content.appendChild(createSectionElement(
        'leoAssistant',
        'leoAssistant',
        'settings-hns-leo-assistant-page',
        {
          prefs: '{{prefs}}'
        }
      ))
      const sectionNewTab = document.createElement('template')
      sectionNewTab.setAttribute('is', 'dom-if')
      sectionNewTab.setAttribute('restamp', true)
      sectionNewTab.setAttribute('if', '[[showPage_(pageVisibility.newTab)]]')
      sectionNewTab.content.appendChild(createSectionElement(
        'newTab',
        'hnsNewTab',
        'settings-hns-new-tab-page',
        {
          prefs: '{{prefs}}'
        }
      ))

      // Remove all hidden performance options from basic page.
      // We moved performance elements in system settings.
      const performanceTemplate = actualTemplate.content.querySelector(
        'template[if="[[showPerformancePage_(pageVisibility.performance)]]"]')
      if (performanceTemplate) {
        performanceTemplate.remove()
      }

      const batteryTemplate = actualTemplate.content.querySelector(
        'template[if="[[showBatteryPage_(pageVisibility.performance)]]"]')
      if (batteryTemplate) {
        batteryTemplate.remove()
      }

      // Get Started at top
      let last = basicPageEl.insertAdjacentElement('afterbegin',
        sectionGetStarted)
      // Move Appearance item
      const sectionAppearance = getSectionElement(actualTemplate.content,
        'appearance')
      last = last.insertAdjacentElement('afterend', sectionAppearance)
      // Insert New Tab
      last = last.insertAdjacentElement('afterend', sectionNewTab)
      // Insert shields
      last = last.insertAdjacentElement('afterend', sectionShields)
      // Insert Rewards
      if (sectionRewards) {
        last = last.insertAdjacentElement('afterend', sectionRewards)
      }
      // Insert Social Blocking
      last = last.insertAdjacentElement('afterend', sectionSocialBlocking)
      // Move privacy section to after social blocking
      const sectionPrivacy = getSectionElement(actualTemplate.content, 'privacy')
      last = last.insertAdjacentElement('afterend', sectionPrivacy)
      // Insert sync
      last = last.insertAdjacentElement('afterend', sectionSync)
      // Move search
      const sectionSearch = getSectionElement(actualTemplate.content, 'search')
      last = last.insertAdjacentElement('afterend', sectionSearch)
      // Insert extensions
      last = last.insertAdjacentElement('afterend', sectionExtensions)
      // Insert Wallet
      if (sectionWallet) {
        last = last.insertAdjacentElement('afterend', sectionWallet)
      }
      // Insert IPFS
      last = last.insertAdjacentElement('afterend', sectionIPFS)
      // Insert Web3 Domains
      last = last.insertAdjacentElement('afterend', sectionWeb3Domains)
      // Insert Tor
      last = last.insertAdjacentElement('afterend', sectionTor)
      // Insert Leo Assistant
      last = last.insertAdjacentElement('afterend', sectionLeoAssist)

      // Advanced
      const advancedTemplate = templateContent.querySelector('template[if="[[showAdvancedSettings_(pageVisibility.advancedSettings)]]"]')
      if (!advancedTemplate) {
        console.error('[Hns Settings Overrides] Could not find advanced section')
      }
      const advancedSubSectionsTemplate = advancedTemplate.content.querySelector('settings-idle-load template')
      if (!advancedSubSectionsTemplate) {
        console.error('[Hns Settings Overrides] Could not find advanced sub-sections container')
      }
      // Move autofill to before languages
      const sectionAutofill = getSectionElement(actualTemplate.content, 'autofill')
      const sectionLanguages = getSectionElement(advancedSubSectionsTemplate.content, 'languages')
      sectionLanguages.insertAdjacentElement('beforebegin', sectionAutofill)
      // Move help tips after downloads
      const sectionDownloads = getSectionElement(advancedSubSectionsTemplate.content, 'downloads')
      sectionDownloads.insertAdjacentElement('afterend', sectionHelpTips)
    }
  }
})
