// Copyright (c) 2020 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

import * as React from 'react'

import {
  FeaturedSettingsWidget,
  StyledBannerImage,
  StyledSettingsInfo,
  StyledSettingsTitle,
  StyledSettingsCopy,
  StyledWidgetToggle,
  SettingsWidget,
  StyledAddButtonIcon,
  StyledHideButtonIcon,
  StyledWidgetSettings,
  StyledButtonLabel,
  ToggleCardsWrapper,
  ToggleCardsTitle,
  ToggleCardsCopy,
  ToggleCardsSwitch,
  ToggleCardsText
} from '../../../components/default'
import hnsTalkBanner from './assets/hns-talk.png'
import rewardsBanner from './assets/hnsrewards.png'
import HideIcon from './assets/hide-icon'
import Toggle from '@hns/leo/react/toggle'
import { PlusIcon } from 'hns-ui/components/icons'

import { getLocale } from '../../../../common/locale'

interface Props {
  toggleShowHnsTalk: () => void
  showHnsTalk: boolean
  hnsTalkSupported: boolean
  toggleShowRewards: () => void
  showRewards: boolean
  hnsRewardsSupported: boolean
  toggleCards: (show: boolean) => void
  cardsHidden: boolean
}

class CardsSettings extends React.PureComponent<Props, {}> {
  renderToggleButton = (on: boolean, toggleFunc: any, float: boolean = true) => {
    const ButtonContainer = on ? StyledHideButtonIcon : StyledAddButtonIcon
    const ButtonIcon = on ? HideIcon : PlusIcon

    return (
      <StyledWidgetToggle
        isAdd={!on}
        float={float}
        onClick={toggleFunc}
      >
        <ButtonContainer>
          <ButtonIcon />
        </ButtonContainer>
        <StyledButtonLabel>
          {
            on
            ? getLocale('hideWidget')
            : getLocale('addWidget')
          }
        </StyledButtonLabel>
      </StyledWidgetToggle>
    )
  }

  render () {
    const {
      toggleShowHnsTalk,
      showHnsTalk,
      hnsTalkSupported,
      toggleShowRewards,
      showRewards,
      hnsRewardsSupported,
      toggleCards,
      cardsHidden
    } = this.props
    return (
      <StyledWidgetSettings>
        {
          hnsTalkSupported
          ? <FeaturedSettingsWidget>
              <StyledBannerImage src={hnsTalkBanner} />
              <StyledSettingsInfo>
                <StyledSettingsTitle>
                  {getLocale('hnsTalkWidgetTitle')}
                </StyledSettingsTitle>
                <StyledSettingsCopy>
                  {getLocale('hnsTalkWidgetWelcomeTitle')}
                </StyledSettingsCopy>
              </StyledSettingsInfo>
              {this.renderToggleButton(showHnsTalk, toggleShowHnsTalk)}
            </FeaturedSettingsWidget>
          : null
        }
        {
          hnsRewardsSupported
            ? <SettingsWidget>
              <StyledBannerImage src={rewardsBanner} />
              <StyledSettingsInfo>
                <StyledSettingsTitle>
                  {getLocale('hnsRewardsTitle')}
                </StyledSettingsTitle>
                <StyledSettingsCopy>
                  {getLocale('rewardsWidgetDesc')}
                </StyledSettingsCopy>
              </StyledSettingsInfo>
              {this.renderToggleButton(showRewards, toggleShowRewards, false)}
            </SettingsWidget>
            : null
        }
        <FeaturedSettingsWidget>
          <ToggleCardsWrapper>
            <ToggleCardsText>
              <ToggleCardsTitle>
                {getLocale('cardsToggleTitle')}
              </ToggleCardsTitle>
              <ToggleCardsCopy>
                {getLocale('cardsToggleDesc')}
              </ToggleCardsCopy>
            </ToggleCardsText>
            <ToggleCardsSwitch>
              <Toggle
                size='small'
                onChange={toggleCards.bind(this, cardsHidden)}
                checked={!cardsHidden}
              />
            </ToggleCardsSwitch>
          </ToggleCardsWrapper>
        </FeaturedSettingsWidget>
      </StyledWidgetSettings>
    )
  }
}

export default CardsSettings
