// Copyright (c) 2020 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

import * as React from 'react'

import {
  SettingsRow,
  SettingsText
} from '../../../components/default'
import Toggle from '@hns/leo/react/toggle'

import { getLocale } from '../../../../common/locale'
import { useNewTabPref } from '../../../hooks/usePref'

export default function HnsStatsSettings () {
  const [showStats, setShowStats] = useNewTabPref('showStats')
  return <div>
    <SettingsRow>
      <SettingsText>{getLocale('showHnsStats')}</SettingsText>
      <Toggle
        onChange={() => setShowStats(!showStats)}
        checked={showStats}
        size='small'
      />
    </SettingsRow>
  </div>
}
