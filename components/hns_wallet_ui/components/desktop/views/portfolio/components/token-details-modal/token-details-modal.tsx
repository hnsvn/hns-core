// Copyright (c) 2022 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at https://mozilla.org/MPL/2.0/.

import * as React from 'react'

// types
import { HnsWallet } from '../../../../../../constants/types'

// utils
import { getLocale } from '../../../../../../../common/locale'

// components
import CopyTooltip from '../../../../../shared/copy-tooltip/copy-tooltip'
import withPlaceholderIcon from '../../../../../shared/create-placeholder-icon'
import { Skeleton } from '../../../../../shared/loading-skeleton/styles'

// styles
import PopupModal from '../../../../../desktop/popup-modals'
import {
  StyledWrapper,
  TokenBalanceRow,
  CryptoBalance,
  DetailColumn,
  TokenDetailLabel,
  TokenDetailValue,
  FiatBalance,
  ContractAddress,
  HideTokenButton,
  modalWidth
} from './token-details-modal-styles'
import { AssetIcon } from '../../style'

interface Props {
  selectedAsset: HnsWallet.BlockchainToken
  selectedAssetNetwork: HnsWallet.NetworkInfo
  assetBalance?: string
  formattedFiatBalance: string
  onClose: () => void
  onShowHideTokenModal: () => void
}

const AssetIconWithPlaceholder = withPlaceholderIcon(AssetIcon, { size: 'big' })

export const TokenDetailsModal = (props: Props) => {
  const {
    selectedAsset,
    selectedAssetNetwork,
    assetBalance,
    formattedFiatBalance,
    onClose,
    onShowHideTokenModal
  } = props

  return (
    <>
      <PopupModal
        title={getLocale('hnsWalletPortfolioTokenDetailsMenuLabel')}
        onClose={onClose}
        width={modalWidth}
      >
        <StyledWrapper>
          <TokenBalanceRow>
            <CryptoBalance>{assetBalance}</CryptoBalance>
            <AssetIconWithPlaceholder asset={selectedAsset} network={selectedAssetNetwork} />
          </TokenBalanceRow>
          <FiatBalance>{formattedFiatBalance || <Skeleton width='20%' />}</FiatBalance>
          <DetailColumn>
            <TokenDetailLabel>{getLocale('hnsWalletTokenContractAddress')}</TokenDetailLabel>
            <CopyTooltip text={selectedAsset.contractAddress}>
              <ContractAddress>{selectedAsset.contractAddress}</ContractAddress>
            </CopyTooltip>
          </DetailColumn>
          <DetailColumn>
            <TokenDetailLabel>{getLocale('hnsWalletTokenDecimal')}</TokenDetailLabel>
            <TokenDetailValue>{selectedAsset.decimals}</TokenDetailValue>
          </DetailColumn>
          <DetailColumn>
            <TokenDetailLabel>{getLocale('hnsWalletTransactionDetailNetwork')}</TokenDetailLabel>
            <TokenDetailValue>{selectedAssetNetwork.chainName}</TokenDetailValue>
          </DetailColumn>
          <HideTokenButton onClick={onShowHideTokenModal}>{getLocale('hnsWalletHideTokenModalTitle')}</HideTokenButton>
        </StyledWrapper>
      </PopupModal>
    </>
  )
}
