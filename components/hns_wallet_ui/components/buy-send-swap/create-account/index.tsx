// Copyright (c) 2022 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at https://mozilla.org/MPL/2.0/.

import * as React from 'react'

// Redux
import { useDispatch } from 'react-redux'
import { WalletActions } from '../../../common/actions'
import {
  useSafeWalletSelector,
  useUnsafeWalletSelector
} from '../../../common/hooks/use-safe-selector'
import { WalletSelectors } from '../../../common/selectors'

// Types
import { HnsWallet } from '../../../constants/types'

// Components
import { NavButton } from '../../extension/buttons/nav-button/index'
import { LockPanel } from '../../extension/lock-panel'

// Utils
import { getLocale } from '../../../../common/locale'
import { suggestNewAccountName } from '../../../utils/address-utils'
import { keyringIdForNewAccount } from '../../../utils/account-utils'

// Styled Components
import {
  StyledWrapper,
  Description,
  ButtonRow
} from './style'

export interface Props {
  network: HnsWallet.NetworkInfo
  onCreated?: () => void
  onCancel: () => void
}

export const CreateAccountTab = ({
  network: accountNetwork,
  onCreated,
  onCancel,
}: Props) => {
  // redux
  const dispatch = useDispatch()
  const accounts = useUnsafeWalletSelector(WalletSelectors.accounts)
  const isWalletLocked = useSafeWalletSelector(WalletSelectors.isWalletLocked)

  // state
  const [showUnlock, setShowUnlock] = React.useState<boolean>(false)

  const suggestedAccountName = React.useMemo((): string => {
    return accountNetwork
      ? suggestNewAccountName(accounts, accountNetwork)
      : ''
  }, [accounts, accountNetwork])

  const onCreateAccount = React.useCallback(() => {
    // unlock needed to create accounts
    if (isWalletLocked && !showUnlock) {
      return setShowUnlock(true)
    }

    dispatch(
      WalletActions.addAccount({
        coin: accountNetwork.coin,
        keyringId: keyringIdForNewAccount(
          accountNetwork.coin,
          accountNetwork.chainId
        ),
        accountName: suggestedAccountName
      })
    )
    if (onCreated) {
      onCreated()
    }
  }, [
    isWalletLocked,
    showUnlock,
    accountNetwork,
    suggestedAccountName,
    onCreated,
  ])

  const handleUnlockAttempt = React.useCallback((password: string): void => {
    dispatch(WalletActions.unlockWallet({ password }))
    onCreateAccount()
  }, [onCreateAccount])

  // effects
  React.useEffect(() => {
    // hide unlock screen on unlock success
    if (!isWalletLocked && showUnlock) {
      setShowUnlock(false)
    }
  }, [isWalletLocked, showUnlock])

  // render
  if (isWalletLocked && showUnlock) {
    return <StyledWrapper>
      <Description style={{ fontSize: 16 }}>
        {getLocale('hnsWalletUnlockNeededToCreateAccount')}
      </Description>
      <LockPanel
        hideBackground
        onSubmit={handleUnlockAttempt}
      />
    </StyledWrapper>
  }

  return (
    <StyledWrapper>
      <Description>
        {accountNetwork
          ? getLocale('hnsWalletCreateAccountDescription').replace('$1', accountNetwork.symbolName)
          : ''
        }
      </Description>

      <ButtonRow>
        <NavButton
          buttonType='secondary'
          onSubmit={onCancel}
          text={getLocale('hnsWalletCreateAccountNo')}
        />
        <NavButton
          buttonType='primary'
          onSubmit={onCreateAccount}
          text={getLocale('hnsWalletCreateAccountYes')}
        />
      </ButtonRow>
    </StyledWrapper>
  )
}

export default CreateAccountTab
