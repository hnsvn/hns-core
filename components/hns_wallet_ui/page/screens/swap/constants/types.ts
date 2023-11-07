// Copyright (c) 2023 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

import { HnsWallet, SpotPriceRegistry } from '../../../../constants/types'

import Amount from '../../../../utils/amount'

type LiquiditySource = {
  name: string
  proportion: Amount
}

export type QuoteOption = {
  label: string
  fromAmount: Amount
  toAmount: Amount
  minimumToAmount: Amount | undefined
  fromToken: HnsWallet.BlockchainToken
  toToken: HnsWallet.BlockchainToken
  rate: Amount
  impact: Amount
  sources: LiquiditySource[]

  /**
   * Indicates the kind of routing followed by the order:
   *
   * split -> indicates that the order was fulfilled from two separate LPs
   *
   * flow  -> indicates that the order was fulfilled through an intermediate
   *          asset between two separate LPs.
   */
  routing: 'split' | 'flow'

  networkFee: string

  hnsFee: HnsWallet.HnsSwapFeeResponse | undefined
}

export type SwapAndSend = {
  label: string
  name: string
}

export type GasFeeOption = {
  id: string
  name: string
  icon: string
}

export type Exchange = {
  id: string
  name: string
}

export type GasEstimate = {
  gasFee: string
  gasFeeGwei?: string
  gasFeeFiat?: string
  time?: string
}

export type AmountValidationErrorType =
  'fromAmountDecimalsOverflow' | 'toAmountDecimalsOverflow'

export type SwapValidationErrorType =
  | AmountValidationErrorType
  | 'insufficientBalance'
  | 'insufficientFundsForGas'
  | 'insufficientAllowance'
  | 'insufficientLiquidity'
  | 'unknownError'

export type SwapParams = {
  fromToken?: HnsWallet.BlockchainToken
  toToken?: HnsWallet.BlockchainToken
  fromAmount: string
  toAmount: string
  /**
   * This is the value as seen on the UI - should be converted to appropriate
   * format for Jupiter and 0x swap providers.
   */
  slippageTolerance: string
  fromAddress?: string
  spotPrices?: SpotPriceRegistry
}
