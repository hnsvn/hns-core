// Copyright (c) 2022 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at https://mozilla.org/MPL/2.0/.

import * as React from 'react'
import { useDispatch } from 'react-redux'
import { skipToken } from '@reduxjs/toolkit/query/react'

// Hooks
import { useExplorer } from '../../../common/hooks/explorer'
import {
  useGetAccountInfosRegistryQuery,
  useGetDefaultFiatCurrencyQuery,
  useGetNetworkQuery,
  useGetSolanaEstimatedFeeQuery,
  useGetTokenSpotPricesQuery,
  walletApi
} from '../../../common/slices/api.slice'
import {
  useUnsafeUISelector
} from '../../../common/hooks/use-safe-selector'
import {
  useAccountQuery,
  useGetCombinedTokensListQuery,
  useTransactionQuery
} from '../../../common/slices/api.slice.extra'
import {
  querySubscriptionOptions60s
} from '../../../common/slices/constants'
import { useAccountOrb, useAddressOrb } from '../../../common/hooks/use-orb'

// Utils
import { makeNetworkAsset } from '../../../options/asset-options'
import { reduceAddress } from '../../../utils/reduce-address'
import {
  getTransactionGasFee,
  getTransactionStatusString,
  getTransactionToAddress,
  isFilecoinTransaction,
  isSolanaTransaction,
  isSwapTransaction,
  parseTransactionWithPrices,
  findTransactionToken,
  getETHSwapTransactionBuyAndSellTokens
} from '../../../utils/tx-utils'
import { getPriceIdForToken } from '../../../utils/api-utils'
import { toProperCase } from '../../../utils/string-utils'
import Amount from '../../../utils/amount'
import { getCoinFromTxDataUnion } from '../../../utils/network-utils'
import { getLocale } from '../../../../common/locale'
import { UISelectors } from '../../../common/selectors'
import { serializedTimeDeltaToJSDate } from '../../../utils/datetime-utils'

// Constants
import {
  HnsWallet
} from '../../../constants/types'

// Styled Components
import {
  StyledWrapper,
  OrbContainer,
  FromCircle,
  ToCircle,
  DetailRow,
  DetailTitle,
  DetailButton,
  StatusRow,
  BalanceColumn,
  TransactionValue,
  PanelDescription,
  SpacerText,
  FromToRow,
  AccountNameText,
  ArrowIcon,
  AlertIcon
} from './style'
import {
  DetailTextDarkBold,
  DetailTextDark
} from '../shared-panel-styles'
import Header from '../../buy-send-swap/select-header'
import { StatusBubble } from '../../shared/style'
import { TransactionStatusTooltip } from '../transaction-status-tooltip'
import { Tooltip } from '../../shared/tooltip/index'
import { Skeleton } from '../../shared/loading-skeleton/styles'

interface Props {
  transactionId: string
  visibleTokens: HnsWallet.BlockchainToken[]
  onBack: () => void
}

export const TransactionDetailPanel = (props: Props) => {
  // props
  const {
    transactionId,
    onBack,
  } = props

  // redux
  const dispatch = useDispatch()
  const transactionProviderErrorRegistry = useUnsafeUISelector(
    UISelectors.transactionProviderErrorRegistry
  )

  // queries & query args
  const { data: defaultFiatCurrency } = useGetDefaultFiatCurrencyQuery()
  const { data: combinedTokensList } = useGetCombinedTokensListQuery()
  const { transaction } = useTransactionQuery(transactionId || skipToken)
  const txCoinType = transaction
    ? getCoinFromTxDataUnion(transaction.txDataUnion)
    : undefined
  const { data: accounts } = useGetAccountInfosRegistryQuery()
  const { account } = useAccountQuery(transaction?.fromAccountId)

  const isSolanaTxn = transaction
    ? isSolanaTransaction(transaction)
    : undefined

  const { data: transactionsNetwork } = useGetNetworkQuery(
    transaction && txCoinType
      ? {
          chainId: transaction.chainId,
          coin: txCoinType
        }
      : skipToken
  )

  const { data: solFeeEstimate } = useGetSolanaEstimatedFeeQuery(
    isSolanaTxn && transaction?.chainId && transaction?.id
      ? {
          chainId: transaction.chainId,
          txId: transaction.id
        }
      : skipToken
  )

  const networkAsset = React.useMemo(() => {
    return makeNetworkAsset(transactionsNetwork)
  }, [transactionsNetwork])

  const txToken = findTransactionToken(transaction, combinedTokensList)
  const { buyToken, sellToken } = getETHSwapTransactionBuyAndSellTokens({
    tx: transaction,
    tokensList: combinedTokensList,
    nativeAsset: networkAsset
  })

  const tokenPriceIds = React.useMemo(
    () =>
      [txToken, networkAsset, buyToken, sellToken]
        .filter((t): t is HnsWallet.BlockchainToken => Boolean(t))
        .map(getPriceIdForToken),
    [txToken, networkAsset, buyToken, sellToken]
  )

  const {
    data: spotPriceRegistry = {}
  } = useGetTokenSpotPricesQuery(
    tokenPriceIds.length && defaultFiatCurrency
      ? { ids: tokenPriceIds, toCurrency: defaultFiatCurrency }
      : skipToken,
    querySubscriptionOptions60s
  )

  // memos
  const gasFee = React.useMemo(() => {
    if (!transaction) {
      return ''
    }

    return txCoinType === HnsWallet.CoinType.SOL
      ? solFeeEstimate ?? ''
      : getTransactionGasFee(transaction)
  }, [transaction, txCoinType, solFeeEstimate])

  const transactionDetails = React.useMemo(() => {
    if (
      !transaction ||
      !spotPriceRegistry ||
      !transactionsNetwork ||
      !account ||
      !accounts
    ) {
      return undefined
    }

    return parseTransactionWithPrices({
      tx: transaction,
      accounts,
      gasFee,
      spotPriceRegistry,
      tokensList: combinedTokensList,
      transactionAccount: account,
      transactionNetwork: transactionsNetwork
    })
  }, [
    transaction,
    account,
    transactionsNetwork,
    accounts,
    spotPriceRegistry,
    gasFee
  ])

  const { txType } = transaction || {}
  const {
    erc721BlockchainToken,
    fiatValue,
    gasFeeFiat,
    isApprovalUnlimited,
    value: normalizedTransferredValue,
    recipient,
    recipientLabel,
    senderLabel,
    symbol,
  } = transactionDetails || {}

  const fromOrb = useAccountOrb(account)
  const toOrb = useAddressOrb(getTransactionToAddress(transaction))

  const transactionTitle = React.useMemo((): string => {
    if (!transaction) {
      return ''
    }
    if (isSwapTransaction(transaction)) {
      return toProperCase(getLocale('hnsWalletSwap'))
    }
    if (transaction?.txType === HnsWallet.TransactionType.ERC20Approve) {
      return toProperCase(getLocale('hnsWalletApprovalTransactionIntent'))
    }
    return toProperCase(getLocale('hnsWalletTransactionSent'))
  }, [transaction])

  const transactionValue = React.useMemo((): string => {
    if (txType !== undefined || !normalizedTransferredValue) {
      return ''
    }

    if (
      txType === HnsWallet.TransactionType.ERC721TransferFrom ||
      txType === HnsWallet.TransactionType.ERC721SafeTransferFrom
    ) {
      return erc721BlockchainToken?.name + ' ' + erc721BlockchainToken?.tokenId
    }

    if (
      txType === HnsWallet.TransactionType.ERC20Approve &&
      isApprovalUnlimited
    ) {
      return `${getLocale('hnsWalletTransactionApproveUnlimited')} ${symbol}`
    }

    return new Amount(normalizedTransferredValue).formatAsAsset(
      undefined,
      symbol
    )
  }, [
    erc721BlockchainToken,
    isApprovalUnlimited,
    symbol,
    txType,
    normalizedTransferredValue
  ])

  const transactionFiatValue = React.useMemo((): string => {
    if (!txType !== undefined || !fiatValue) {
      return ''
    }

    if (
      txType !== HnsWallet.TransactionType.ERC721TransferFrom &&
      txType !== HnsWallet.TransactionType.ERC721SafeTransferFrom &&
      txType !== HnsWallet.TransactionType.ERC20Approve
    ) {
      return new Amount(fiatValue).formatAsFiat(defaultFiatCurrency)
    }
    return ''
  }, [fiatValue, txType, defaultFiatCurrency])

  const isFilTransaction = transaction
    ? isFilecoinTransaction(transaction)
    : undefined
  const txError = transactionProviderErrorRegistry[transactionId]

  // methods
  const onClickViewOnBlockExplorer = useExplorer(transactionsNetwork)

  const onClickRetryTransaction = () => {
    if (transaction && txCoinType) {
      dispatch(
        walletApi.endpoints.retryTransaction.initiate({
          chainId: transaction.chainId,
          coinType: txCoinType,
          transactionId: transaction.id
        })
      )
    }
  }

  const onClickSpeedupTransaction = () => {
    if (transaction && txCoinType) {
      dispatch(
        walletApi.endpoints.speedupTransaction.initiate({
          chainId: transaction.chainId,
          coinType: txCoinType,
          transactionId: transaction.id
        })
      )
    }
  }

  const onClickCancelTransaction = () => {
    if (transaction && txCoinType) {
      dispatch(
        walletApi.endpoints.cancelTransaction.initiate({
          chainId: transaction.chainId,
          coinType: txCoinType,
          transactionId: transaction.id
        })
      )
    }
  }

  // render
  if (!transaction || !account) {
    return <Skeleton />
  }

  return (
    <StyledWrapper>
      <Header
        title={getLocale('hnsWalletTransactionDetails')}
        onBack={onBack}
      />
      <OrbContainer>
        <FromCircle orb={fromOrb} />
        <ToCircle orb={toOrb} />
      </OrbContainer>
      <FromToRow>
        <Tooltip
          // TODO(apaymyshev): handle bitcoin transaction
          text={transaction.fromAddress ?? ''}
          isAddress={true}
          position={'left'}
        >
          <AccountNameText>{senderLabel}</AccountNameText>
        </Tooltip>
        <ArrowIcon />
        <Tooltip text={recipient} isAddress={true} position={'right'}>
          <AccountNameText>{recipientLabel}</AccountNameText>
        </Tooltip>
      </FromToRow>
      <PanelDescription>{transactionTitle}</PanelDescription>
      <TransactionValue>{transactionValue}</TransactionValue>
      <PanelDescription>{transactionFiatValue}</PanelDescription>
      <DetailRow>
        <DetailTitle>
          {getLocale('hnsWalletTransactionDetailStatus')}
        </DetailTitle>
        <StatusRow>
          <StatusBubble status={transaction?.txStatus} />
          <DetailTextDarkBold>
            {getTransactionStatusString(transaction?.txStatus)}
          </DetailTextDarkBold>

          {transaction?.txStatus === HnsWallet.TransactionStatus.Error &&
            txError && (
              <TransactionStatusTooltip
                text={`${txError.code}: ${txError.message}`}
              >
                <AlertIcon />
              </TransactionStatusTooltip>
            )}
        </StatusRow>
      </DetailRow>
      {/* Will remove this conditional for solana once https://github.com/hnsvn/hns-browser/issues/22040 is implemented. */}
      {!isSolanaTxn && (
        <DetailRow>
          <DetailTitle>
            {getLocale('hnsWalletAllowSpendTransactionFee')}
          </DetailTitle>
          <BalanceColumn>
            <DetailTextDark>
              {gasFee && transactionsNetwork ? (
                new Amount(gasFee)
                  .divideByDecimals(transactionsNetwork.decimals)
                  .formatAsAsset(6, transactionsNetwork.symbol)
              ) : (
                <Skeleton />
              )}
            </DetailTextDark>
            <DetailTextDark>
              {gasFeeFiat ? (
                new Amount(gasFeeFiat).formatAsFiat(defaultFiatCurrency)
              ) : (
                <Skeleton />
              )}
            </DetailTextDark>
          </BalanceColumn>
        </DetailRow>
      )}
      <DetailRow>
        <DetailTitle>
          {getLocale('hnsWalletTransactionDetailDate')}
        </DetailTitle>
        <DetailTextDark>
          {serializedTimeDeltaToJSDate(transaction.createdTime).toUTCString()}
        </DetailTextDark>
      </DetailRow>
      {![
        HnsWallet.TransactionStatus.Rejected,
        HnsWallet.TransactionStatus.Error
      ].includes(transaction?.txStatus) && (
        <DetailRow>
          <DetailTitle>
            {getLocale('hnsWalletTransactionDetailHash')}
          </DetailTitle>
          <DetailButton
            onClick={onClickViewOnBlockExplorer('tx', transaction?.txHash)}
          >
            {reduceAddress(transaction?.txHash)}
          </DetailButton>
        </DetailRow>
      )}
      <DetailRow>
        <DetailTitle>
          {getLocale('hnsWalletTransactionDetailNetwork')}
        </DetailTitle>
        <DetailTextDark>{transactionsNetwork?.chainName ?? ''}</DetailTextDark>
      </DetailRow>

      {[
        HnsWallet.TransactionStatus.Approved,
        HnsWallet.TransactionStatus.Submitted
      ].includes(transaction?.txStatus) &&
        !isSolanaTxn &&
        !isFilTransaction && (
          <DetailRow>
            <DetailTitle />
            <StatusRow>
              <DetailButton onClick={onClickSpeedupTransaction}>
                {getLocale('hnsWalletTransactionDetailSpeedUp')}
              </DetailButton>
              <SpacerText>|</SpacerText>
              <DetailButton onClick={onClickCancelTransaction}>
                {getLocale('hnsWalletButtonCancel')}
              </DetailButton>
            </StatusRow>
          </DetailRow>
        )}
      {transaction?.txStatus === HnsWallet.TransactionStatus.Error &&
        !isSolanaTxn &&
        !isFilTransaction && (
          <DetailRow>
            <DetailTitle />
            <StatusRow>
              <DetailButton onClick={onClickRetryTransaction}>
                {getLocale('hnsWalletTransactionRetry')}
              </DetailButton>
            </StatusRow>
          </DetailRow>
        )}
    </StyledWrapper>
  )
}

export default TransactionDetailPanel
