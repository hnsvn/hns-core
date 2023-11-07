// Copyright (c) 2021 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at https://mozilla.org/MPL/2.0/.
import * as React from 'react'
import Button from '$web-components/button'

import * as S from './style'
import { ButtonText, IconBox } from '../general'
import { AlertCircleIcon, CaratStrongLeftIcon } from 'hns-ui/components/icons'
import { getLocale } from '../../../../../common/locale'
import { useSelector, useDispatch } from '../../state/hooks'
import * as Actions from '../../state/actions'

interface Props {
  showContactSupport: React.MouseEventHandler<HTMLButtonElement>
}

function ErrorPanel (props: Props) {
  const dispatch = useDispatch()
  const currentRegion = useSelector(state => state.currentRegion)

  const handleShowMainView = () => {
    dispatch(Actions.resetConnectionState())
  }

  const handleTryAgain = () => {
    dispatch(Actions.connect())
  }

  const handleChooseServer = () => {
    dispatch(Actions.toggleRegionSelector(true))
  }

  const matches = {
    $1: getLocale('hnsVpn'),
    $2: currentRegion?.namePretty || ''
  }

  return (
    <S.Box>
      <S.PanelContent>
        <S.PanelHeader>
          <S.BackButton
            type='button'
            onClick={handleShowMainView}
            aria-label='Go to main'
          >
            <i><CaratStrongLeftIcon /></i>
            <span>{getLocale('hnsVpnErrorPanelHeader')}</span>
          </S.BackButton>
        </S.PanelHeader>
        <IconBox>
          <AlertCircleIcon color='#84889C' />
        </IconBox>
        <S.ReasonTitle>{getLocale('hnsVpnUnableConnectToServer')}</S.ReasonTitle>
        <S.ReasonDesc>
          {getLocale('hnsVpnUnableConnectInfo').replace(/\$\d+/g, (match) => matches[match])}
        </S.ReasonDesc>
        <S.ActionArea>
          <Button
            type={'submit'}
            isPrimary
            isCallToAction
            onClick={handleTryAgain}
          >
            {getLocale('hnsVpnTryAgain')}
          </Button>
          <ButtonText onClick={handleChooseServer}>
            {getLocale('hnsVpnChooseAnotherServer')}
          </ButtonText>
          <ButtonText onClick={props.showContactSupport}>
            {getLocale('hnsVpnContactSupport')}
          </ButtonText>
        </S.ActionArea>
      </S.PanelContent>
    </S.Box>
  )
}

export default ErrorPanel
