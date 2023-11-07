// Copyright (c) 2021 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at https://mozilla.org/MPL/2.0/.
import * as React from 'react'

// Types
import {
  HnsWallet,
  OrderTypes,
  SlippagePresetObjectType,
  ExpirationPresetObjectType,
  SwapValidationErrorType,
  AmountPresetTypes,
  DefaultCurrencies,
} from '../../../constants/types'

// Options
import { AmountPresetOptions } from '../../../options/amount-preset-options'
import { SlippagePresetOptions } from '../../../options/slippage-preset-options'
import { ExpirationPresetOptions } from '../../../options/expiration-preset-options'

// Utils
import { getLocale } from '../../../../common/locale'
import { reduceAddress } from '../../../utils/reduce-address'
import { CurrencySymbols } from '../../../utils/currency-symbols'
import Amount from '../../../utils/amount'
import { WalletSelectors } from '../../../common/selectors'

// Hooks
import {
  useUnsafeWalletSelector //
} from '../../../common/hooks/use-safe-selector'

// Components
import {
  withPlaceholderIcon //
} from '../../shared/create-placeholder-icon/index'
import { Tooltip } from '../../shared/tooltip/index'

// Styled Components
import {
  RefreshButton,
  RefreshIcon,
  FromBalanceText,
  AssetIcon,
  AssetButton,
  AssetTicker,
  CaratDownIcon,
  PresetButton,
  PresetRow,
  SelectValueText,
  SelectText,
  Input,
  Row,
  PasteIcon,
  PasteButton,
  SlippageInput,
  WarningText,
  AddressConfirmationText,
  ButtonLeftSide,
  LearnMoreButton,
  WarningRow,
  Spacer
} from './style'

import { BubbleContainer } from '../shared-styles'


export type BuySendSwapInputType =
  | 'toAmount'
  | 'fromAmount'
  | 'toAddress'
  | 'buyAmount'
  | 'exchange'
  | 'selector'

export interface Props {
  autoFocus?: boolean
  componentType: BuySendSwapInputType
  selectedAssetBalance?: string
  selectedAsset?: HnsWallet.BlockchainToken | undefined
  selectedNetwork?: HnsWallet.NetworkInfo
  selectedAssetInputAmount?: string
  addressError?: string
  addressWarning?: string
  toAddressOrUrl?: string
  toAddress?: string
  inputName?: string
  orderType?: OrderTypes
  slippageTolerance?: SlippagePresetObjectType
  orderExpiration?: ExpirationPresetObjectType
  validationError?: SwapValidationErrorType | undefined
  customSlippageTolerance?: string
  defaultCurrencies?: DefaultCurrencies
  selectedPreset?: AmountPresetTypes | undefined
  isV2?: boolean
  onCustomSlippageToleranceChange?: (value: string) => void
  onInputChange?: (value: string, name: string) => void
  onSelectPresetAmount?: (percent: number) => void
  onSelectSlippageTolerance?: (slippage: SlippagePresetObjectType) => void
  onSelectExpiration?: (expiration: ExpirationPresetObjectType) => void
  onToggleOrderType?: () => void
  onShowSelection?: () => void
  onRefresh?: () => void
  onPaste?: () => void
  onShowCurrencySelection?: () => void
}

const onClickLearnMore = () => {
  chrome.tabs.create(
    { url: 'https://support.hns.com/hc/en-us/articles/4441999049101' },
    () => {
      if (chrome.runtime.lastError) {
        console.error(
          'tabs.create failed: ' + //
            chrome.runtime.lastError.message
        )
      }
    }
  )
}

function SwapInputComponent (props: Props) {
  const {
    autoFocus,
    selectedAsset,
    selectedNetwork,
    selectedAssetBalance,
    componentType,
    selectedAssetInputAmount,
    inputName,
    addressError,
    addressWarning,
    toAddressOrUrl,
    toAddress,
    orderType,
    slippageTolerance,
    orderExpiration,
    validationError,
    customSlippageTolerance,
    selectedPreset,
    isV2,
    onCustomSlippageToleranceChange,
    onInputChange,
    onPaste,
    onRefresh,
    onSelectPresetAmount,
    onSelectSlippageTolerance,
    onSelectExpiration,
    onShowSelection,
    onShowCurrencySelection
  } = props
  const [spin, setSpin] = React.useState<number>(0)
  const [expandSelector, setExpandSelector] = React.useState<boolean>(false)
  const [showSlippageWarning, setShowSlippageWarning] = React.useState<boolean>(false)

  // redux
  const reduxSelectedCurrency = useUnsafeWalletSelector(
    WalletSelectors.selectedCurrency
  )
  const currencies = useUnsafeWalletSelector(WalletSelectors.onRampCurrencies)

  // methods
  const toggleExpandSelector = () => {
    setExpandSelector(!expandSelector)
  }

  const refresh = () => {
    if (onRefresh) {
      onRefresh()
    }
    setSpin(1)
  }

  const setPresetSlippageValue = (slippage: SlippagePresetObjectType) => () => {
    if (onSelectSlippageTolerance) {
      onSelectSlippageTolerance(slippage)
      setExpandSelector(false)
    }
  }

  const setExpirationValue = (expiration: ExpirationPresetObjectType) => () => {
    if (onSelectExpiration) {
      onSelectExpiration(expiration)
      setExpandSelector(false)
    }
  }

  const setPresetAmountValue = (percent: AmountPresetTypes) => () => {
    if (onSelectPresetAmount) {
      onSelectPresetAmount(percent)
    }
  }

  const getTitle = () => {
    switch (componentType) {
      case 'fromAmount':
        return getLocale('hnsWalletSwapFrom')
      case 'toAmount':
        if (orderType === 'market') {
          return `${getLocale('hnsWalletSwapTo')} (${getLocale('hnsWalletSwapEstimate')})`
        } else {
          return getLocale('hnsWalletSwapTo')
        }
      case 'buyAmount':
        return ''
      case 'exchange':
        if (orderType === 'market') {
          return `${getLocale('hnsWalletSwapMarket')} ${getLocale('hnsWalletSwapPriceIn')} ${selectedAsset?.symbol}`
        } else {
          return `${getLocale('hnsWalletSwapPriceIn')} ${selectedAsset?.symbol}`
        }
      case 'selector':
        if (orderType === 'market') {
          return getLocale('hnsWalletSlippageToleranceTitle')
        } else {
          return getLocale('hnsWalletExpiresInTitle')
        }
      case 'toAddress':
        return getLocale('hnsWalletSwapTo')
    }
  }

  const onInputChanged = (event: React.ChangeEvent<HTMLInputElement>) => {
    if (onInputChange) {
      onInputChange(event.target.value, event.target.name)
    }
  }

  const handleCustomSlippageToleranceChange = (event: React.ChangeEvent<HTMLInputElement>) => {
    if (onCustomSlippageToleranceChange) {
      // This will only formate to only allow Numbers and remove multiple . decimals
      const value = event.target.value.replace(/[^0-9.]/g, '').replace(/(\..*?)\..*/g, '$1')
      // Sets the value to not go higher than 100
      if (Number(value) > 100) {
        onCustomSlippageToleranceChange('100')
        return
      }
      // Prevents double 00 before decimal place and formats to 0. if value starts with .
      if (value === '00' || value === '.') {
        onCustomSlippageToleranceChange('0.')
        return
      }
      onCustomSlippageToleranceChange(value)
    }
  }

  const resetAnimation = () => {
    setSpin(0)
  }

  const getAssetSymbol = (symbol?: string) => {
    // Ramp assets have the format: ChainNativeTokenSymbol_AssetSymbol e.g MATIC_BSC
    // This returns just the AssetSymbol
    return symbol && symbol.includes('_')
      ? symbol.split('_')[1]
      : symbol
  }

  // memos & computed
  React.useMemo(() => {
    // Show Warning if slippage is to high
    if (Number(customSlippageTolerance) >= 6) {
      setShowSlippageWarning(true)
      return
    }
    setShowSlippageWarning(false)
  }, [customSlippageTolerance])

  const selectedCurrency = React.useMemo(() => {
    return reduxSelectedCurrency || currencies[0]
  }, [reduxSelectedCurrency, currencies])

  const fromAmountHasErrors = validationError && [
    'insufficientBalance',
    'insufficientFundsForGas',
    'fromAmountDecimalsOverflow'
  ].includes(validationError)

  const toAmountHasErrors = validationError && [
    'toAmountDecimalsOverflow'
  ].includes(validationError)

  const AssetIconWithPlaceholder = React.useMemo(() => {
    return withPlaceholderIcon(AssetIcon, { size: 'small', marginLeft: 4, marginRight: 8 })
  }, [])

  const formattedAssetBalance = selectedAssetBalance
    ? getLocale('hnsWalletBalance') + ': ' + new Amount(selectedAssetBalance)
      .divideByDecimals(selectedAsset?.decimals ?? 18)
      .format(6, true)
    : ''

  const placeholderText = React.useMemo((): string => {
    return componentType === 'toAddress'
      ? selectedNetwork?.coin === HnsWallet.CoinType.ETH
        ? getLocale('hnsWalletSendPlaceholder')
        : getLocale('hnsWalletSendNoURLPlaceholder')
      : '0'
  }, [selectedNetwork, componentType])

  // render
  return (
    <BubbleContainer isV2={isV2}>
      {componentType !== 'selector' &&
        <>
          {!(selectedAsset?.isErc721 || selectedAsset?.isNft) &&
            <Row>
              <FromBalanceText componentType={componentType}>{getTitle()}</FromBalanceText>

              {/* Limit orders on Swap are currently disabled.
              componentType === 'exchange' &&
                <MarketLimitButton onClick={onToggleOrderType}>{orderType === 'market' ? locale.swapLimit : locale.swapMarket}</MarketLimitButton>
            */}

              {componentType !== 'exchange' && componentType !== 'toAddress' && componentType !== 'buyAmount' &&
                <FromBalanceText>{formattedAssetBalance}</FromBalanceText>
              }
              {componentType === 'toAddress' &&
                <PasteButton onClick={onPaste}>
                  <PasteIcon />
                </PasteButton>
              }
            </Row>
          }
          <Row componentType={componentType}>
            {componentType === 'buyAmount' &&
              <AssetButton onClick={onShowCurrencySelection}>
                <AssetTicker
                  isV2={isV2}
                  role='currency'
                >
                  {CurrencySymbols[selectedCurrency?.currencyCode]}
                </AssetTicker>
                <CaratDownIcon name='carat-down' />
                <Spacer />
              </AssetButton>
            }
            {!(selectedAsset?.isErc721 || selectedAsset?.isNft) &&
              <Input
                componentType={componentType}
                type={componentType === 'toAddress' ? 'text' : 'number'}
                placeholder={placeholderText}
                value={componentType === 'toAddress' ? toAddressOrUrl : selectedAssetInputAmount}
                name={inputName}
                onChange={onInputChanged}
                spellCheck={false}
                hasError={
                  (componentType === 'fromAmount' && !!fromAmountHasErrors) ||
                  (componentType === 'toAmount' && !!toAmountHasErrors)
                }
                disabled={
                  (orderType === 'market' && componentType === 'exchange') ||
                  (orderType === 'limit' && componentType === 'toAmount') ||
                  (selectedNetwork?.chainId === HnsWallet.SOLANA_MAINNET && componentType === 'toAmount')
                }
                autoFocus={autoFocus}
                isV2={isV2}
              />
            }
            {componentType === 'exchange' && orderType === 'market' &&
              <RefreshButton onClick={refresh}>
                <RefreshIcon
                  onAnimationEnd={resetAnimation}
                  spin={spin}
                />
              </RefreshButton>
            }
            {componentType !== 'exchange' && componentType !== 'toAddress' &&
              <AssetButton isERC721={(selectedAsset?.isErc721 || selectedAsset?.isNft)} onClick={onShowSelection}>
                <ButtonLeftSide>
                  <AssetIconWithPlaceholder asset={selectedAsset} network={selectedNetwork} />
                  <AssetTicker
                    isV2={isV2}
                    role='symbol'
                  >
                    {getAssetSymbol(selectedAsset?.symbol)} {
                      selectedAsset?.isErc721 && selectedAsset?.tokenId
                        ? '#' + new Amount(selectedAsset.tokenId).toNumber()
                        : ''
                    }
                  </AssetTicker>
                </ButtonLeftSide>
                {onShowSelection && <CaratDownIcon />}
              </AssetButton>
            }
          </Row>
          {componentType === 'fromAmount' && !(selectedAsset?.isErc721 || selectedAsset?.isNft) &&
            <PresetRow>
              {AmountPresetOptions().map((preset, idx) =>
                <PresetButton
                  isSelected={selectedPreset === preset.value}
                  key={idx}
                  onClick={setPresetAmountValue(preset.value)}
                >
                  {preset.name}
                </PresetButton>
              )}
            </PresetRow>
          }
        </>
      }
      {componentType === 'selector' &&
        <>
          <Row>
            <SelectText>{getTitle()}</SelectText>
            <AssetButton onClick={toggleExpandSelector}>
              <SelectValueText>{orderType === 'market' ? customSlippageTolerance ? `${customSlippageTolerance}%` : `${slippageTolerance?.slippage}%` : `${orderExpiration?.expiration} days`}</SelectValueText>
              <CaratDownIcon />
            </AssetButton>
          </Row>
          {expandSelector &&
            <PresetRow>
              {orderType === 'market' ? (
                <>
                  {SlippagePresetOptions.map((preset) =>
                    <PresetButton
                      key={preset.id}
                      isSlippage={true}
                      isSelected={customSlippageTolerance === '' ? slippageTolerance?.slippage === preset.slippage : false}
                      onClick={setPresetSlippageValue(preset)}
                    >
                      {preset.slippage}%
                    </PresetButton>
                  )}
                  <SlippageInput
                    value={customSlippageTolerance}
                    placeholder='%'
                    type='text'
                    isSelected={customSlippageTolerance !== ''}
                    onChange={handleCustomSlippageToleranceChange}
                    maxLength={4}
                  />
                </>
              ) : (
                <>
                  {ExpirationPresetOptions.map((preset) =>
                    <PresetButton
                      key={preset.id}
                      onClick={setExpirationValue(preset)}
                    >
                      {preset.name}
                    </PresetButton>
                  )}
                </>
              )}
            </PresetRow>
          }
        </>
      }

      {componentType === 'fromAmount' && validationError === 'fromAmountDecimalsOverflow' &&
        <WarningText>{getLocale('hnsWalletDecimalPlacesError')}</WarningText>
      }
      {componentType === 'toAmount' && validationError === 'toAmountDecimalsOverflow' &&
        <WarningText>{getLocale('hnsWalletDecimalPlacesError')}</WarningText>
      }
      {showSlippageWarning &&
        <WarningText>{getLocale('hnsWalletSlippageToleranceWarning')}</WarningText>
      }
      {componentType === 'toAddress' && addressError &&
        <WarningRow>
          <WarningText>{addressError}</WarningText>
          {addressError === getLocale('hnsWalletNotValidChecksumAddressError') &&
            <LearnMoreButton onClick={onClickLearnMore}>
              {getLocale('hnsWalletAllowAddNetworkLearnMoreButton')}
            </LearnMoreButton>
          }
        </WarningRow>
      }
      {componentType === 'toAddress' && addressWarning &&
        <WarningRow>
          <WarningText isWarning={true}>{addressWarning}</WarningText>
          <LearnMoreButton onClick={onClickLearnMore}>
            {getLocale('hnsWalletAllowAddNetworkLearnMoreButton')}
          </LearnMoreButton>
        </WarningRow>
      }
      {componentType === 'toAddress' && toAddress !== toAddressOrUrl && !addressError &&
        <>
          <Tooltip
              text={toAddress ?? ''}
              isAddress={true}
            >
              <AddressConfirmationText>{reduceAddress(toAddress ?? '')}</AddressConfirmationText>
            </Tooltip>
        </>
      }
    </BubbleContainer >
  )
}

export default SwapInputComponent
