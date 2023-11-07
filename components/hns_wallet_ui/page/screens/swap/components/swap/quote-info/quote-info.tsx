// Copyright (c) 2023 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

import * as React from 'react'

// Types
import {
  HnsWallet,
  SpotPriceRegistry
} from '../../../../../../constants/types'
import {
  QuoteOption
} from '../../../constants/types'

// Constants
import LPMetadata from '../../../constants/LpMetadata'

// Utils
import Amount from '../../../../../../utils/amount'
import {
  getLocale
} from '../../../../../../../common/locale'
import {
  getTokenPriceAmountFromRegistry
} from '../../../../../../utils/pricing-utils'

// Styled Components
import {
  HnsFeeContainer,
  HnsFeeDiscounted,
  Bubble,
  ExpandButton,
  FuelTank,
  HorizontalArrows,
  LPIcon,
  LPSeparator,
  LPRow
} from './quote-info.style'
import {
  Column,
  Row,
  Text,
  VerticalSpacer,
  HorizontalSpacer,
  Icon
} from '../../shared-swap.styles'

interface Props {
  selectedQuoteOption: QuoteOption | undefined
  fromToken: HnsWallet.BlockchainToken | undefined
  toToken: HnsWallet.BlockchainToken | undefined
  toAmount: string
  spotPrices?: SpotPriceRegistry
}

export const QuoteInfo = (props: Props) => {
  const { selectedQuoteOption, fromToken, toToken, spotPrices } = props

  // State
  const [showProviders, setShowProviders] = React.useState<boolean>(false)

  // Memos
  const swapRate: string = React.useMemo(() => {
    if (selectedQuoteOption === undefined) {
      return ''
    }

    return `1 ${selectedQuoteOption
      .fromToken.symbol
      } ≈ ${selectedQuoteOption
        .rate
        .format(6)
      } ${selectedQuoteOption
        .toToken
        .symbol
      }`
  }, [selectedQuoteOption])

  const coinGeckoDelta: Amount = React.useMemo(() => {
    if (
      fromToken !== undefined &&
      toToken !== undefined &&
      spotPrices &&
      !getTokenPriceAmountFromRegistry(spotPrices, fromToken).isUndefined() &&
      !getTokenPriceAmountFromRegistry(spotPrices, toToken).isUndefined() &&
      selectedQuoteOption !== undefined
    ) {
      // Exchange rate is the value <R> in the following equation:
      // 1 FROM = <R> TO

      // CoinGecko rate computation:
      //   1 FROM = <R> TO
      //   1 FROM/USD = <R> TO/USD
      //   => <R> = (FROM/USD) / (TO/USD)
      const coinGeckoRate = getTokenPriceAmountFromRegistry(
        spotPrices,
        fromToken
      ).div(getTokenPriceAmountFromRegistry(spotPrices, toToken))

      // Quote rate computation:
      //   <X> FROM = <Y> TO
      //   1 FROM = <R> TO
      //   => <R> = <Y>/<X>
      const quoteRate = selectedQuoteOption.rate

      // The trade is profitable if quoteRate > coinGeckoRate.
      return quoteRate.minus(coinGeckoRate).div(quoteRate).times(100)
    }

    return Amount.zero()
  }, [spotPrices, fromToken, toToken, selectedQuoteOption])

  const coinGeckoDeltaText: string = React.useMemo(() => {
    if (coinGeckoDelta.gte(0)) {
      return getLocale('hnsSwapCoinGeckoCheaper')
        .replace('$1', coinGeckoDelta.format(2))
    }

    if (coinGeckoDelta.gte(-1)) {
      return getLocale('hnsSwapCoinGeckoWithin')
        .replace('$1', coinGeckoDelta.times(-1).format(2))
    }

    return getLocale('hnsSwapCoinGeckoExpensive').replace(
      '$1',
      coinGeckoDelta.times(-1).format(2)
    )
  }, [coinGeckoDelta, getLocale])

  const coinGeckoDeltaColor = React.useMemo(() => {
    if (coinGeckoDelta.gte(-1)) {
      return 'success'
    }

    if (coinGeckoDelta.gte(-5)) {
      return 'warning'
    }

    return 'error'
  }, [coinGeckoDelta])

  const swapImpact: string = React.useMemo(() => {
    if (selectedQuoteOption === undefined) {
      return ''
    }
    return selectedQuoteOption.impact.format(6)
  }, [selectedQuoteOption])

  const minimumReceived: string = React.useMemo(() => {
    if (
      selectedQuoteOption === undefined ||
      selectedQuoteOption.minimumToAmount === undefined
    ) {
      return ''
    }

    return selectedQuoteOption
      .minimumToAmount
      .formatAsAsset(6, selectedQuoteOption.toToken.symbol)
  }, [selectedQuoteOption])

  const hnsFee = React.useMemo(() => {
    if (!selectedQuoteOption?.hnsFee) {
      return undefined
    }

    const { hnsFee: hnsFeeOriginal } = selectedQuoteOption

    return {
      ...hnsFeeOriginal,
      effectiveFeePct: new Amount(hnsFeeOriginal.effectiveFeePct).format(6),
      protocolFeePct: new Amount(hnsFeeOriginal.protocolFeePct),
      discountOnHnsFeePct: new Amount(hnsFeeOriginal.discountOnHnsFeePct).format(6),
      hnsFeePct: new Amount(hnsFeeOriginal.hnsFeePct).format(6)
    }
  }, [selectedQuoteOption])

  return (
    <Column columnHeight='dynamic' columnWidth='full'>
      <VerticalSpacer size={16} />
      <Row rowWidth='full' marginBottom={10} horizontalPadding={16}>
        <Text textSize='14px'>{getLocale('hnsSwapRate')}</Text>
        <Row>
          <Text textSize='14px'>{swapRate}</Text>
          <HorizontalArrows name='swap-horizontal' size={16} />
        </Row>
      </Row>
      <Row rowWidth='full' marginBottom={10} horizontalPadding={16}>
        <HorizontalSpacer size={1} />
        <Row>
          <Text textSize='14px' textColor={coinGeckoDeltaColor}>
            {coinGeckoDeltaText}
          </Text>
        </Row>
      </Row>
      <Row rowWidth='full' marginBottom={10} horizontalPadding={16}>
        <Text textSize='14px'>{getLocale('hnsSwapPriceImpact')}</Text>
        <Text
          textSize='14px'
        >
          {swapImpact === '0' ? `${swapImpact}%` : `~ ${swapImpact}%`}
        </Text>
      </Row>
      {minimumReceived !== '' && (
        <Row rowWidth='full' marginBottom={8} horizontalPadding={16}>
          <Text textSize='14px' textAlign='left'>
            {getLocale('hnsSwapMinimumReceivedAfterSlippage')}
          </Text>
          <Text textSize='14px' textAlign='right'>
            {minimumReceived}
          </Text>
        </Row>
      )}
      {selectedQuoteOption && selectedQuoteOption.sources.length > 0 && (
        <Column columnWidth='full' marginBottom={8} horizontalPadding={16}>
          <Row rowWidth='full' marginBottom={8}>
            <Text textSize='14px' textAlign='left'>
              {getLocale('hnsSwapLiquidityProvider')}
            </Text>
            <Row>
              <Text textSize='14px'>
                {selectedQuoteOption.sources.length}
              </Text>
              <HorizontalSpacer size={8} />
              <ExpandButton
                isExpanded={showProviders}
                onClick={() => setShowProviders(prev => !prev)}
              >
                <Icon
                  size={14}
                  name='carat-down'
                />
              </ExpandButton>
            </Row>
          </Row>
          {showProviders &&
            <LPRow
              rowWidth='full'
              horizontalAlign='flex-start'
              verticalPadding={6}
            >
              {selectedQuoteOption.sources.map((source, idx) => (
                <Row key={idx}>
                  <Bubble>
                    <Text
                      textSize='12px'
                    >
                      {source.name.split('_').join(' ')}
                    </Text>
                    {LPMetadata[source.name] ? (
                      <LPIcon icon={LPMetadata[source.name]} size={12} />
                    ) : null}
                  </Bubble>

                  {idx !== selectedQuoteOption.sources.length - 1 && (
                    <LPSeparator textSize='14px'>
                      {selectedQuoteOption.routing === 'split' ? '+' : '×'}
                    </LPSeparator>
                  )}
                </Row>
              ))}
            </LPRow>
          }
        </Column>
      )}
      {selectedQuoteOption && (
        <Row rowWidth='full' marginBottom={8} horizontalPadding={16}>
          <Text textSize='14px'>{getLocale('hnsSwapNetworkFee')}</Text>
          <Bubble>
            <FuelTank name='search-fuel-tank' size={16} />
            <Text textSize='14px'>{selectedQuoteOption.networkFee}</Text>
          </Bubble>
        </Row>
      )}
      {hnsFee && (
        <Row rowWidth='full' marginBottom={8} horizontalPadding={16}>
          <Text textSize='14px'>{getLocale('hnsSwapHnsFee')}</Text>
          <Text textSize='14px'>
            <HnsFeeContainer>
              {hnsFee.discountCode === HnsWallet.DiscountCode.kNone && (
                <Text textSize='14px'>
                  {hnsFee.effectiveFeePct}%
                </Text>
              )}

              {hnsFee.discountCode !== HnsWallet.DiscountCode.kNone && (
                <>
                  {!hnsFee.hasHnsFee
                    ? (
                      <Text textSize='14px' textColor='success' isBold={true}>
                        {getLocale('hnsSwapFree')}
                      </Text>
                    )
                    : (
                      <Text textSize='14px'>
                        {hnsFee.effectiveFeePct}%
                      </Text>
                    )
                  }

                  <HnsFeeDiscounted textSize='14px' textColor='text03'>
                    {hnsFee.hnsFeePct}%
                  </HnsFeeDiscounted>

                  {hnsFee.hasHnsFee &&
                    <Text textSize='14px'>
                     (-{hnsFee.discountOnHnsFeePct}%)
                    </Text>
                  }
                </>
              )}
            </HnsFeeContainer>
          </Text>
        </Row>
      )}

      {hnsFee && !hnsFee.protocolFeePct.isZero() && (
        <Row rowWidth='full' marginBottom={16} horizontalPadding={16}>
          <Text textSize='14px'>{getLocale('hnsSwapProtocolFee')}</Text>
          <Text textSize='14px'>
            <HnsFeeContainer>
              <Text textSize='14px'>
                {hnsFee.protocolFeePct.format(6)}%
              </Text>
            </HnsFeeContainer>
          </Text>
        </Row>
      )}
    </Column>
  )
}
