// Copyright (c) 2023 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

import * as React from 'react'
import { getLocale } from '../../../../common/locale'
import { HnsWallet } from '../../../constants/types'

import { ErrorTooltip } from './components/error-tooltip/error-tooltip'

import {
  StyledWrapper,
  ContentWrapper,
  UploadIcon,
  CloseIcon,
  CheckIcon,
  Text
} from './nft-pinning-status.style'

interface Props {
  pinningStatusCode: HnsWallet.TokenPinStatusCode
}

export const NftPinningStatus = (props: Props) => {
  const { pinningStatusCode } = props
  // state
  const [icon, setIcon] = React.useState<React.ReactNode>()
  const [message, setmessage] = React.useState<string>('')
  const [showTooltip, setShowTooltip] = React.useState<boolean>(false)

  // methods
  const onToggleErrorTooltip = React.useCallback(() => {
    if (pinningStatusCode === HnsWallet.TokenPinStatusCode.STATUS_PINNING_FAILED) {
      setShowTooltip(showTooltip => !showTooltip)
    }
  }, [pinningStatusCode])


  // effects
  React.useEffect(() => {
    switch (pinningStatusCode) {
      case HnsWallet.TokenPinStatusCode.STATUS_PINNING_IN_PROGRESS:
      case HnsWallet.TokenPinStatusCode.STATUS_PINNING_PENDING:
        setmessage(getLocale('hnsWalletNftPinningStatusPinning'))
        setIcon(<UploadIcon />)
        break

      case HnsWallet.TokenPinStatusCode.STATUS_PINNING_FAILED:
        setmessage(getLocale('hnsWalletNftPinningStatusPinningFailed'))
        setIcon(<CloseIcon />)
        break

      case HnsWallet.TokenPinStatusCode.STATUS_PINNED:
        setmessage(getLocale('hnsWalletNftPinningStatusPinned'))
        setIcon(<CheckIcon />)
        break
    }
  }, [pinningStatusCode])

  return (
    <StyledWrapper>
      <ContentWrapper
        pinningStatus={pinningStatusCode}
        onMouseEnter={onToggleErrorTooltip}
        onMouseLeave={onToggleErrorTooltip}
      >
        {icon}
        <Text>{message}</Text>
      </ContentWrapper>
      {showTooltip &&
        <ErrorTooltip />
      }
    </StyledWrapper>
  )
}
