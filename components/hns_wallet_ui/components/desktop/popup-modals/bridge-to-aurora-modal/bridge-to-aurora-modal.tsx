// Copyright (c) 2022 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at https://mozilla.org/MPL/2.0/.

import * as React from 'react'
import { getLocale } from '../../../../../common/locale'

import { PopupModal } from '../../popup-modals/index'
import {
  ButtonText,
  Description,
  GlobeIcon,
  LearnMoreLink,
  OpenRainbowAppButton,
  StyledWrapper,
  Title,
  CheckboxWrapper
} from './bridge-to-aurora-modal-styles'
import { Checkbox } from '../../../shared/checkbox/checkbox'

interface Props {
  dontShowWarningAgain: boolean
  onClose: () => void
  onOpenRainbowAppClick: () => void
  onDontShowAgain: (selected: boolean) => void
}

const learnMoreLink = 'https://doc.aurora.dev/bridge/bridge-overview/'
const learnMoreRiskMitigation = 'https://rainbowbridge.app/approvals'

export const BridgeToAuroraModal = ({ dontShowWarningAgain, onClose, onOpenRainbowAppClick, onDontShowAgain }: Props) => {
  return (
    <PopupModal
      title=''
      onClose={onClose}
    >
      <StyledWrapper>
        <Title>{getLocale('hnsWalletAuroraModalTitle')}</Title>
        <Description>
          {getLocale('hnsWalletAuroraModalDescription')}
        </Description>
        <CheckboxWrapper>
          <Checkbox isChecked={dontShowWarningAgain} onChange={onDontShowAgain}>
            {getLocale('hnsWalletAuroraModalDontShowAgain')}
          </Checkbox>
        </CheckboxWrapper>
        <OpenRainbowAppButton
          onClick={onOpenRainbowAppClick}
        >
          <GlobeIcon />
          <ButtonText>{getLocale('hnsWalletAuroraModalOPenButtonText')}</ButtonText>
        </OpenRainbowAppButton>
        <LearnMoreLink
          rel='noopener noreferrer'
          target='_blank'
          href={learnMoreLink}
        >
          {getLocale('hnsWalletAuroraModalLearnMore')}
        </LearnMoreLink>
        <LearnMoreLink
          rel='noopener noreferrer'
          target='_blank'
          href={learnMoreRiskMitigation}
        >
          {getLocale('hnsWalletAuroraModalLearnMoreAboutRisk')}
        </LearnMoreLink>
      </StyledWrapper>
    </PopupModal>
  )
}