// Copyright (c) 2022 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

import * as React from 'react'

import * as S from './style'
import { getLocale } from '$web-common/locale'
import Icon from '@hns/leo/react/icon'

interface ReaderModeControlProps {
  onClose?: Function
}

function ReaderModeControl(props: ReaderModeControlProps) {
  return (
    <S.Box>
      <S.Caption>
        <Icon name='product-readermode' />
        {getLocale('hnsReaderModeCaption')}
      </S.Caption>
      <S.CloseButton id="close" onClick={() => { props.onClose?.() }} title={getLocale('hnsReaderModeExit')}>
        <Icon name='close' />
      </S.CloseButton>
    </S.Box>
  )
}

export default ReaderModeControl
