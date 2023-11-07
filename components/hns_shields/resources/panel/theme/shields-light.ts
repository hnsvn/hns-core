/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

 import ITheme from 'hns-ui/theme/theme-interface'
 import defaultTheme from 'hns-ui/theme/hns-default'
 import IThemeShields from './shields-theme'

 const shieldsLightTheme: ITheme & IThemeShields = {
   ...defaultTheme,
   name: 'Shields light',
   color: {
     ...defaultTheme.color,
     background03: '#F8F9FA',
     text01: '#212529',
     warningIcon: '#EA3A0D',
     interactive05: '#4C54D2',
     interactive06: '#737ADE',
     interactive07: '#4C54D2',
     interactive08: '#353DAB'
   }
 }

 export default shieldsLightTheme
