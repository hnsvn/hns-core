// Copyright (c) 2022 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at https://mozilla.org/MPL/2.0/.

import * as React from 'react'
import { useDispatch, useSelector } from 'react-redux'

// utils
import { getLocale } from '../../../../../common/locale'

// types
import { PageState, WalletRoutes } from '../../../../constants/types'

// actions
import { WalletPageActions } from '../../../actions'

// components
import { CenteredPageLayout } from '../../../../components/desktop/centered-page-layout/centered-page-layout'
import { StepsNavigation } from '../../../../components/desktop/steps-navigation/steps-navigation'

// styles
import { WalletLink } from '../../../../components/shared/style'

import {
  StyledWrapper,
  Title,
  Description,
  MainWrapper
} from '../onboarding.style'

import {
  HnsWalletIcon,
 CardButton,
 CardButtonTextContainer,
 LegacyWalletIcon,
 LinkRow,
 MetaMaskIcon
} from './import-or-restore-wallet.style'

export const OnboardingImportOrRestoreWallet = () => {
  // redux
  const dispatch = useDispatch()
  const importWalletError = useSelector(({ page }: { page: PageState }) => page.importWalletError)
  const isImportWalletsCheckComplete = useSelector(({ page }: { page: PageState }) => page.isImportWalletsCheckComplete)
  const isMetaMaskInitialized = useSelector(({ page }: { page: PageState }) => page.isMetaMaskInitialized)
  const isLegacyCryptoWalletsInitialized = useSelector(({ page }: { page: PageState }) => page.isCryptoWalletsInitialized)

  // effects
  React.useEffect(() => {
    // reset any pending import errors
    if (importWalletError?.hasError) {
      dispatch(WalletPageActions.setImportWalletError({
        hasError: false
      }))
    }
  }, [importWalletError?.hasError])

  React.useEffect(() => {
    if (!isImportWalletsCheckComplete) {
      // check if MM or legacy wallet is installed
      dispatch(WalletPageActions.checkWalletsToImport())
    }
  }, [isImportWalletsCheckComplete])

  // render
  return (
    <CenteredPageLayout>
      <MainWrapper>
        <StyledWrapper>

          <StepsNavigation
            goBackUrl={WalletRoutes.Onboarding}
            currentStep={''}
            steps={[]}
          />

          <div>
            <Title>
              {getLocale('hnsWalletImportOrRestoreWalletTitle')}
            </Title>
            <Description>
              {getLocale('hnsWalletImportOrRestoreDescription')}
            </Description>
          </div>

          <CardButton
            to={WalletRoutes.OnboardingRestoreWallet}
          >
            <CardButtonTextContainer>
              <p>
                {getLocale('hnsWalletRestoreMyHnsWallet')}
              </p>
              <p>
                {getLocale('hnsWalletRestoreMyHnsWalletDescription')}
              </p>
            </CardButtonTextContainer>
            <HnsWalletIcon />
          </CardButton>

          {isMetaMaskInitialized && <CardButton
            to={WalletRoutes.OnboardingImportMetaMask}
          >
            <CardButtonTextContainer>
              <p>
                {getLocale('hnsWalletImportFromMetaMask')}
              </p>
              <p>
                {getLocale('hnsWalletImportFromMetaMaskDescription')}
              </p>
            </CardButtonTextContainer>
            <MetaMaskIcon />
          </CardButton>}

          {isLegacyCryptoWalletsInitialized &&
            <CardButton
              to={WalletRoutes.OnboardingImportCryptoWallets}
            >
              <CardButtonTextContainer>
                <p>
                  {getLocale('hnsWalletImportFromLegacy')}
                </p>
              </CardButtonTextContainer>
              <LegacyWalletIcon />
            </CardButton>
          }

          <LinkRow>
            <WalletLink
              to={WalletRoutes.OnboardingCreatePassword}
            >
              {getLocale('hnsWalletCreateWalletInsteadLink')}
            </WalletLink>
          </LinkRow>

        </StyledWrapper>
      </MainWrapper>
    </CenteredPageLayout>
  )
}

export default OnboardingImportOrRestoreWallet
