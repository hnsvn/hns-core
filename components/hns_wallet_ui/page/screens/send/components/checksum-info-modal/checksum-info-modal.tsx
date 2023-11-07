// Copyright (c) 2023 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

import * as React from 'react'

// Assets
import CloseIcon from '../../../../../assets/svg-icons/close.svg'
import ChecksumInfoGraphic from '../../assets/checksum-info-graphic.png'

// Utils
import { getLocale } from '../../../../../../common/locale'

// Styled Components
import { StyledWrapper, Modal, InfoColumn, InfoGraphic, Link } from './checksum-info-modal.style'
import { Column, Text, VerticalSpacer, IconButton, Row } from '../../shared.styles'

interface Props {
  onClose: () => void
}

export const ChecksumInfoModal = React.forwardRef<HTMLDivElement, Props>(
  (props: Props, forwardedRef) => {
    const { onClose } = props

    return (
      <StyledWrapper>
        <Modal ref={forwardedRef}>
          <VerticalSpacer size={38} />
          <Row
            rowWidth='full'
            horizontalAlign='flex-end'
            horizontalPadding={32}
          >
            <IconButton icon={CloseIcon} onClick={onClose} size={18} />
          </Row>
          <VerticalSpacer size={8} />
          <Column
            columnWidth='full'
            verticalAlign='center'
            horizontalAlign='center'
            horizontalPadding={40}
          >
            <Text
              textSize='22px'
              textColor='text01'
              isBold={true}
            >
              {getLocale('hnsWalletChecksumModalTitle')}
            </Text>
            <VerticalSpacer size={16} />
            <Text
              textSize='12px'
              textColor='text03'
              isBold={false}
            >
              {getLocale('hnsWalletChecksumModalDescription')}
            </Text>
          </Column>
          <VerticalSpacer size={24} />
          <InfoColumn
            columnWidth='full'
            verticalAlign='center'
            horizontalAlign='flex-start'
            horizontalPadding={40}
            verticalPadding={24}
          >
            <Text
              textSize='14px'
              textColor='text01'
              isBold={true}
            >
              {getLocale('hnsWalletChecksumModalStepOneTitle')}{' '}
              <Link
                href='https://etherscan.io'
                target='_blank'
                rel='noopener noreferrer'
              >
                https://etherscan.io
              </Link>
            </Text>
            <VerticalSpacer size={8} />
            <Text
              textSize='12px'
              textColor='text02'
              isBold={false}
              textAlign='left'
            >
              {getLocale('hnsWalletChecksumModalStepOneDescription')}
            </Text>
            <VerticalSpacer size={24} />
            <Text
              textSize='14px'
              textColor='text01'
              isBold={true}
            >
              {getLocale('hnsWalletChecksumModalStepTwoTitle')}
            </Text>
            <VerticalSpacer size={8} />
            <Text
              textSize='12px'
              textColor='text02'
              isBold={false}
              textAlign='left'
            >
              {getLocale('hnsWalletChecksumModalStepTwoDescription')}
            </Text>
            <VerticalSpacer size={8} />
            <InfoGraphic src={ChecksumInfoGraphic} />
          </InfoColumn>
          <Column
            columnWidth='full'
            verticalAlign='center'
            horizontalAlign='center'
            verticalPadding={52}
          >
            <Text
              textSize='14px'
              textColor='text02'
              isBold={true}
            >
              {getLocale('hnsWalletChecksumModalNeedHelp')}{' '}
              <Link
                href='https://support.hns.com/hc/en-us/articles/4415497656461-Hns-Wallet-FAQ'
                target='_blank'
                rel='noopener noreferrer'
              >
                {getLocale('hnsWalletHelpCenter')}
              </Link>
            </Text>
          </Column>
        </Modal>
      </StyledWrapper>
    )
  }
)
