// Copyright (c) 2022 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at https://mozilla.org/MPL/2.0/.
import * as React from 'react'

// Constants
import {
  HnsWallet,
  SerializableTransactionInfo
} from '../../../../constants/types'

// Hooks
import { useExplorer } from '../../../../common/hooks/explorer'
import { useGetSelectedChainQuery } from '../../../../common/slices/api.slice'

// Utils
import { getLocale } from '$web-common/locale'

// Styled components
import { Panel } from '../../panel/index'
import { SubmittedOrSignedIcon, Title } from './submitted_or_signed.style'
import {
  ButtonRow,
  DetailButton,
  LinkIcon,
  TransactionStatusDescription
} from '../common/common.style'

interface Props {
  headerTitle: string
  transaction: SerializableTransactionInfo
  onClose: () => void
}

export const TransactionSubmittedOrSigned = (props: Props) => {
  const { headerTitle, transaction, onClose } = props

  // queries
  const { data: selectedNetwork } = useGetSelectedChainQuery()

  // custom hooks
  const onClickViewOnBlockExplorer = useExplorer(selectedNetwork)

  const title =
    transaction.txStatus === HnsWallet.TransactionStatus.Submitted
      ? getLocale('hnsWalletTransactionSubmittedTitle')
      : getLocale('hnsWalletTransactionSignedTitle')
  const description =
    transaction.txStatus === HnsWallet.TransactionStatus.Submitted
     ? getLocale('hnsWalletTransactionSubmittedDescription')
     : getLocale('hnsWalletTransactionSignedDescription')

  return (
    <Panel navAction={onClose} title={headerTitle} headerStyle='slim'>
      <SubmittedOrSignedIcon />
      <Title>{title}</Title>
      <TransactionStatusDescription>
        {description}
      </TransactionStatusDescription>
      <ButtonRow>
        <DetailButton onClick={onClickViewOnBlockExplorer('tx', transaction.txHash)}>
          {getLocale('hnsWalletTransactionExplorer')}
        </DetailButton>
        <LinkIcon />
      </ButtonRow>
    </Panel>
  )
}
