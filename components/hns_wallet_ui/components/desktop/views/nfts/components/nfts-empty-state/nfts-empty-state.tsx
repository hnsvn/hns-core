// Copyright (c) 2023 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.
import * as React from 'react'

// utils
import { getLocale } from '../../../../../../../common/locale'

// styles
import {
  DisclaimerText,
  EmptyStateImage,
  Heading,
  StyledWrapper,
  SubHeading,
  ImportButton
} from './nfts-empty-state.style'
import EmptyStateGraphicLight from '../../../../../../assets/png-icons/nft-empty-state-light.png'
import EmptyStateGraphicDark from '../../../../../../assets/png-icons/nft-empty-state-dark.png'

interface Props {
  onImportNft: () => void
}

export const NftsEmptyState = ({ onImportNft }: Props) => (
  <StyledWrapper>
    <EmptyStateImage
      src={
        window.matchMedia('(prefers-color-scheme: dark)').matches
          ? EmptyStateGraphicDark
          : EmptyStateGraphicLight
      }
    />
    <Heading>{getLocale('hnsNftsTabEmptyStateHeading')}</Heading>
    <SubHeading>{getLocale('hnsNftsTabEmptyStateSubHeading')}</SubHeading>
    <ImportButton onClick={onImportNft}>
      {getLocale('hnsNftsTabImportNft')}
    </ImportButton>
    <DisclaimerText>
      {getLocale('hnsNftsTabEmptyStateDisclaimer')}
    </DisclaimerText>
  </StyledWrapper>
)
