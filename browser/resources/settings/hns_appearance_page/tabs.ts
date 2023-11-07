// Copyright (c) 2023 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

import '../settings_shared.css.js'
import '../settings_vars.css.js'

import {PrefsMixin, PrefsMixinInterface} from 'chrome://resources/cr_components/settings_prefs/prefs_mixin.js';
import {I18nMixin, I18nMixinInterface} from 'chrome://resources/cr_elements/i18n_mixin.js'
import {PolymerElement} from 'chrome://resources/polymer/v3_0/polymer/polymer_bundled.min.js'

import {loadTimeData} from '../i18n_setup.js'

import {getTemplate} from './tabs.html.js'

const SettingsHnsAppearanceTabsElementBase = PrefsMixin(I18nMixin(PolymerElement)) as {
  new (): PolymerElement & I18nMixinInterface & PrefsMixinInterface
}

export class SettingsHnsAppearanceTabsElement extends SettingsHnsAppearanceTabsElementBase {
  static get is() {
    return 'settings-hns-appearance-tabs'
  }

  static get template() {
    return getTemplate()
  }

  private isVerticalTabStripFeatureEnabled() {
    return loadTimeData.getBoolean('verticalTabStripFeatureEnabled')
  }

  private tabTooltipModes_ = [
    { value: 1, name: this.i18n('appearanceSettingsTabHoverModeCard') },
    {
      value: 2,
      name: this.i18n('appearanceSettingsTabHoverModeCardWithPreview')
    },
    { value: 0, name: this.i18n('appearanceSettingsTabHoverModeTooltip') }
  ]

}

customElements.define(SettingsHnsAppearanceTabsElement.is, SettingsHnsAppearanceTabsElement)
