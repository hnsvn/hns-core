// Copyright (c) 2022 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at https://mozilla.org/MPL/2.0/.

import * as React from 'react'
import { useDispatch } from 'react-redux'

// Actions
import { WalletActions } from '../../../common/actions'

// Types
import { HnsWallet } from '../../../constants/types'

// Hooks
import {
  useUnsafeWalletSelector
} from '../../../common/hooks/use-safe-selector'

// Utils
import { reduceAccountDisplayName } from '../../../utils/reduce-account-name'
import { getLocale } from '../../../../common/locale'
import { reduceAddress } from '../../../utils/reduce-address'
import { useSetSelectedAccountMutation } from '../../../common/slices/api.slice'
import { useSelectedAccountQuery } from '../../../common/slices/api.slice.extra'
import { WalletSelectors } from '../../../common/selectors'

// Hooks
import { useAccountOrb } from '../../../common/hooks/use-orb'

// Styled Components
import {
  StyledWrapper,
  AccountAddressText,
  AccountNameText,
  NameAndAddressColumn,
  AccountCircle,
  LeftSide,
  PrimaryButton,
  RightSide
} from './style'


export interface Props {
  account: HnsWallet.AccountInfo
}

export const ConnectedAccountItem = (props: Props) => {
  const {
    account
  } = props

  const dispatch = useDispatch()
  const connectedAccounts = useUnsafeWalletSelector(WalletSelectors.connectedAccounts)

  // api
  const { data: selectedAccount } = useSelectedAccountQuery()
  const [setSelectedAccount] = useSetSelectedAccountMutation()
  const selectedCoin = selectedAccount?.accountId.coin

  // memos
  const orb = useAccountOrb(account)

  const isActive = account.accountId.uniqueKey === selectedAccount?.accountId.uniqueKey

  const hasPermission = React.useMemo((): boolean => {
    return connectedAccounts.some(
      (accountId) => accountId.uniqueKey === account.accountId.uniqueKey
    )  }, [connectedAccounts, account])

  const buttonText = React.useMemo((): string => {
    if (selectedCoin === HnsWallet.CoinType.SOL) {
      return hasPermission
        ? getLocale('hnsWalletSitePermissionsRevoke')
        : getLocale('hnsWalletSitePermissionsTrust')
    }
    return hasPermission
      ? isActive
        ? getLocale('hnsWalletSitePermissionsDisconnect')
        : getLocale('hnsWalletSitePermissionsSwitch')
      : getLocale('hnsWalletAddAccountConnect')
  }, [selectedCoin, hasPermission, isActive])

  // methods
  const onClickConnect = React.useCallback(() => {
    dispatch(WalletActions.addSitePermission({ accountId: account.accountId }))
    if (selectedCoin !== HnsWallet.CoinType.SOL) {
      setSelectedAccount(account.accountId)
    }
  }, [account, selectedCoin])

  const onClickDisconnect = React.useCallback(() => {
    dispatch(WalletActions.removeSitePermission({ accountId: account.accountId }))
    if (connectedAccounts.length !== 0 && selectedCoin !== HnsWallet.CoinType.SOL) {
      setSelectedAccount(account.accountId)
    }
  }, [connectedAccounts, account, selectedCoin])

  const onClickSwitchAccount = React.useCallback(() => {
    setSelectedAccount(account.accountId)
  }, [account])

  const onClickConnectDisconnectOrSwitch = React.useCallback(() => {
    if (selectedCoin === HnsWallet.CoinType.SOL) {
      return hasPermission
        ? onClickDisconnect()
        : onClickConnect()
    }
    return hasPermission
      ? isActive
        ? onClickDisconnect()
        : onClickSwitchAccount()
      : onClickConnect()
  }, [selectedCoin, hasPermission, isActive, onClickDisconnect, onClickConnect, onClickSwitchAccount])

  return (
    <StyledWrapper>
      <LeftSide>
        <AccountCircle orb={orb} />
        <NameAndAddressColumn>
          <AccountNameText>{reduceAccountDisplayName(account.name, 22)}</AccountNameText>
          <AccountAddressText>
            {reduceAddress(account.address)}
          </AccountAddressText>
        </NameAndAddressColumn>
      </LeftSide>
      <RightSide>
        <PrimaryButton
          onClick={onClickConnectDisconnectOrSwitch}
        >
          {buttonText}
        </PrimaryButton>
      </RightSide>
    </StyledWrapper>
  )
}

export default ConnectedAccountItem
