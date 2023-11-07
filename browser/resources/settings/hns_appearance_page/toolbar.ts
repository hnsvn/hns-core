// Copyright (c) 2020 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at https://mozilla.org/MPL/2.0/.

import {PolymerElement} from 'chrome://resources/polymer/v3_0/polymer/polymer_bundled.min.js'
import {I18nMixin, I18nMixinInterface} from 'chrome://resources/cr_elements/i18n_mixin.js'
import {loadTimeData} from "../i18n_setup.js"
import {getTemplate} from './toolbar.html.js'

import '../settings_shared.css.js'
import '../settings_vars.css.js'
import './sidebar.js'
import './tabs.js'

const SettingsHnsAppearanceToolbarElementBase = I18nMixin(PolymerElement) as {
  new (): PolymerElement & I18nMixinInterface
}

/**
 * 'settings-hns-appearance-toolbar' is the settings page area containing
 * hns's appearance settings related to the toolbar.
 */
class SettingsHnsAppearanceToolbarElement extends SettingsHnsAppearanceToolbarElementBase {
  static get is() {
    return 'settings-hns-appearance-toolbar'
  }

  static get template() {
    return getTemplate()
  }

  // static get properties() {
  //   return {
  //     tabTooltipModes_: {
  //       readOnly: true,
  //       type: Array,
  //       value() {
  //         return [
  //           { value: 1, name: this.i18n('appearanceSettingsTabHoverModeCard') },
  //           {
  //             value: 2,
  //             name: this.i18n('appearanceSettingsTabHoverModeCardWithPreview')
  //           },
  //           { value: 0, name: this.i18n('appearanceSettingsTabHoverModeTooltip') }
  //         ]
  //       }
  //     }
  //   }
  // }

  private showHnsVPNOption_() {
    return loadTimeData.getBoolean('isHnsVPNEnabled')
  }
}

customElements.define(SettingsHnsAppearanceToolbarElement.is, SettingsHnsAppearanceToolbarElement)
