// Copyright (c) 2022 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at https://mozilla.org/MPL/2.0/.

import * as React from 'react'

import { getLocale } from '../../../../common/locale'
import { numberArrayToHexStr } from '../../../utils/hex-utils'
import { HnsWallet, SerializableTransactionInfo } from '../../../constants/types'
import { CodeSnippet, CodeSnippetText, DetailColumn, DetailText, TransactionText } from './style'

export interface Props {
  transactionInfo: SerializableTransactionInfo
}

const txKeys = Object.keys(HnsWallet.TransactionType)

export const TransactionDetailBox = (props: Props) => {
  const { transactionInfo } = props
  const {
    txArgs,
    txParams,
    txType
  } = transactionInfo
  const data = transactionInfo.txDataUnion.ethTxData1559?.baseData.data || []
  return (
    <>
      {data.length === 0 ? (
        <CodeSnippet>
          <code>
            <CodeSnippetText>{getLocale('hnsWalletConfirmTransactionNoData')}</CodeSnippetText>
          </code>
        </CodeSnippet>
      ) : (
        <>
          <DetailColumn>
            <TransactionText>{getLocale('hnsWalletTransactionDetailBoxFunction')}:</TransactionText>
            <DetailText>{txKeys[txType]}</DetailText>
          </DetailColumn>
          {txType !== HnsWallet.TransactionType.Other && txParams.map((param, i) =>
            <CodeSnippet key={i}>
              <code>
                <CodeSnippetText>{param}: {txArgs[i]}</CodeSnippetText>
              </code>
            </CodeSnippet>
          )}

          {txType === HnsWallet.TransactionType.Other && (
            <CodeSnippet>
              <code>
                <CodeSnippetText>
                  {`0x${numberArrayToHexStr(data)}`}
                </CodeSnippetText>
              </code>
            </CodeSnippet>
          )}
        </>
      )}
    </>
  )
}

export default TransactionDetailBox
