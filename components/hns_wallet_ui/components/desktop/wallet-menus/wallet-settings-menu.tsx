// Copyright (c) 2023 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

import * as React from 'react'
import { useDispatch } from 'react-redux'

// Types
import { HnsWallet } from '../../../constants/types'

// actions
import { WalletActions } from '../../../common/actions'

// utils
import { getLocale } from '../../../../common/locale'
import { useGetSelectedChainQuery } from '../../../common/slices/api.slice'

// Styled Components
import {
  StyledWrapper,
  PopupButton,
  PopupButtonText,
  ButtonIcon,
} from './wellet-menus.style'
import {
  VerticalDivider,
  VerticalSpace,
} from '../../shared/style'

export interface Props {
  onClickViewOnBlockExplorer?: () => void
  onClickBackup?: () => void
  onClosePopup?: () => void
  yPosition?: number
}

export const WalletSettingsMenu = (props: Props) => {
  const {
    onClickViewOnBlockExplorer,
    onClickBackup,
    onClosePopup,
    yPosition
  } = props

  // redux
  const dispatch = useDispatch()

  // queries
  const { data: selectedNetwork } = useGetSelectedChainQuery()

  // methods
  const lockWallet = React.useCallback(() => {
    dispatch(WalletActions.lockWallet())
  }, [])

  const onClickConnectedSites = React.useCallback(() => {
    if (!selectedNetwork) {
      return
    }

    const route = selectedNetwork.coin === HnsWallet.CoinType.ETH
      ? 'ethereum'
      : 'solana'

    chrome.tabs.create({ url: `hns://settings/content/${route}` }, () => {
      if (chrome.runtime.lastError) {
        console.error(
          'tabs.create failed: ' +
          chrome.runtime.lastError.message
        )
      }
    })
    if (onClosePopup) {
      onClosePopup()
    }
  }, [selectedNetwork, onClosePopup])

  const onClickHelpCenter = React.useCallback(() => {
    chrome.tabs.create(
      {
        url: 'https://support.hns.com/hc/en-us/categories/360001059151-Hns-Wallet'
      }, () => {
        if (chrome.runtime.lastError) {
          console.error(
            'tabs.create failed: '
            + chrome.runtime.lastError.message
          )
        }
      })
    if (onClosePopup) {
      onClosePopup()
    }
  }, [onClosePopup])

  const onClickSettings = React.useCallback(() => {
    chrome.tabs.create({ url: 'chrome://settings/wallet' }, () => {
      if (chrome.runtime.lastError) {
        console.error(
          'tabs.create failed: ' +
          chrome.runtime.lastError.message
        )
      }
    })
    if (onClosePopup) {
      onClosePopup()
    }
  }, [onClosePopup])

  return (
    <StyledWrapper
      yPosition={yPosition}
    >

      <PopupButton onClick={lockWallet}>
        <ButtonIcon name='lock' />
        <PopupButtonText>
          {getLocale('hnsWalletWalletPopupLock')}
        </PopupButtonText>
      </PopupButton>

      {onClickBackup &&
        <PopupButton onClick={onClickBackup}>
          <ButtonIcon name='safe' />
          <PopupButtonText>
            {getLocale('hnsWalletBackupButton')}
          </PopupButtonText>
        </PopupButton>
      }

      {
        selectedNetwork &&
        selectedNetwork.coin !== HnsWallet.CoinType.FIL &&
        <PopupButton onClick={onClickConnectedSites}>
          <ButtonIcon name='link-normal' />
          <PopupButtonText>
            {getLocale('hnsWalletWalletPopupConnectedSites')}
          </PopupButtonText>
        </PopupButton>
      }

      <PopupButton onClick={onClickSettings}>
        <ButtonIcon name='settings' />
        <PopupButtonText>
          {getLocale('hnsWalletWalletPopupSettings')}
        </PopupButtonText>
      </PopupButton>

      {onClickViewOnBlockExplorer &&
        <PopupButton onClick={onClickViewOnBlockExplorer}>
          <ButtonIcon name='launch' />
          <PopupButtonText>
            {getLocale('hnsWalletTransactionExplorer')}
          </PopupButtonText>
        </PopupButton>
      }

      <VerticalDivider />
      <VerticalSpace space='8px' />

      <PopupButton onClick={onClickHelpCenter}>
        <ButtonIcon name='help-outline' />
        <PopupButtonText>
          {getLocale('hnsWalletHelpCenter')}
        </PopupButtonText>
      </PopupButton>

    </StyledWrapper>
  )
}

export default WalletSettingsMenu
