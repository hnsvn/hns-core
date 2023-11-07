// Copyright (c) 2023 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.
import * as React from 'react'

// utils
import { getLocale, splitStringForTag } from '../../../../../common/locale'

// components
import { PopupModal } from '../../popup-modals/index'

// styles
import {
  ButtonRow,
  CancelButton,
  ConfirmButton,
  Description,
  Header,
  Link,
  StyledWrapper,
  Underline
} from './enable-nft-discovery-modal.style'

interface Props {
  onConfirm: () => void
  onCancel: () => void
}

const LEARN_MORE_LINK = 'https://github.com/hnsvn/hns-browser/wiki/NFT-Discovery'

export const EnableNftDiscoveryModal = ({ onConfirm, onCancel }: Props) => {
  const { beforeTag, duringTag, afterTag } = splitStringForTag(getLocale('hnsWalletEnableNftAutoDiscoveryModalDescription'), 1)
  const { beforeTag: beforeLink, duringTag: learnMore } = splitStringForTag(afterTag || '', 3)

  return (
    <PopupModal
      title=''
      width='477px'
      hideHeader
      onClose={onCancel}
    >
      <StyledWrapper>
        <Header>{getLocale('hnsWalletEnableNftAutoDiscoveryModalHeader')}</Header>
        <Description>
          {beforeTag}
          <Underline>{duringTag}</Underline>
          {beforeLink}
          <Link target='_blank' rel='noreferrer' href={LEARN_MORE_LINK}>{learnMore}</Link>
        </Description>
        <ButtonRow>
          <CancelButton onClick={onCancel}>
            {getLocale('hnsWalletEnableNftAutoDiscoveryModalCancel')}
          </CancelButton>
          <ConfirmButton onClick={onConfirm}>
            {getLocale('hnsWalletEnableNftAutoDiscoveryModalConfirm')}
          </ConfirmButton>
        </ButtonRow>
      </StyledWrapper>
    </PopupModal>
  )
}
