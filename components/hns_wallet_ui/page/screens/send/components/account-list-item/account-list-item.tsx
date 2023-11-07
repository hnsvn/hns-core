// Copyright (c) 2022 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at https://mozilla.org/MPL/2.0/.

import * as React from 'react'

import { HnsWallet } from '../../../../../constants/types'

// Styled Components
import { Button, AccountCircle } from './account-list-item.style'
import { Text, Column } from '../../shared.styles'

// Hooks
import { useAccountOrb } from '../../../../../common/hooks/use-orb'

interface Props {
  account: HnsWallet.AccountInfo
  onClick: (account: HnsWallet.AccountInfo) => void
  isSelected: boolean
  accountAlias: string | undefined
}

export const AccountListItem = (props: Props) => {
  const { onClick, account, isSelected, accountAlias } = props

  // hooks
  const orb = useAccountOrb(account)

  return (
    <Button disabled={isSelected} onClick={() => onClick(account)}>
      <AccountCircle orb={orb} />
      <Column horizontalAlign='flex-start' verticalAlign='center'>
        <Text textColor='text03' textSize='12px' isBold={false}>{account.name}</Text>
        <Text textColor='text01' textSize='12px' isBold={false}>{account.address}</Text>
        {(accountAlias && accountAlias !== '') &&
          <Text textColor='text02' textSize='12px' isBold={false}>{accountAlias}</Text>
        }
      </Column>
    </Button>
  )
}

export default AccountListItem
