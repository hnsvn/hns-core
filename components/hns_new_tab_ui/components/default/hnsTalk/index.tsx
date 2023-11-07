/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

import * as React from 'react'
import createWidget from '../widget/index'
import { getLocale } from '../../../../common/locale'

import {
  WidgetWrapper,
  Header,
  Content,
  WelcomeText,
  ActionsWrapper,
  CallButton,
  HnsTalkIcon,
  StyledTitle,
  Privacy,
  PrivacyLink
} from './style'
import { StyledTitleTab } from '../widgetTitleTab'
import HnsTalkSvg from './assets/hns-talk-svg'
import { hnsTalkWidgetUrl } from '../../../constants/new_tab_ui'

interface Props {
  showContent: boolean
  stackPosition: number
  onShowContent: () => void
}

class HnsTalk extends React.PureComponent<Props, {}> {
  getButtonText = () => {
    return getLocale('hnsTalkWidgetStartButton')
  }

  renderTitle () {
    return (
      <Header>
        <StyledTitle>
          <HnsTalkIcon>
            <HnsTalkSvg />
          </HnsTalkIcon>
          <>
            {getLocale('hnsTalkWidgetTitle')}
          </>
        </StyledTitle>
      </Header>
    )
  }

  renderTitleTab () {
    const { onShowContent, stackPosition } = this.props

    return (
      <StyledTitleTab onClick={onShowContent} stackPosition={stackPosition}>
        {this.renderTitle()}
      </StyledTitleTab>
    )
  }

  shouldCreateCall = (event: any) => {
    event.preventDefault()
    window.open(hnsTalkWidgetUrl, '_self', 'noopener')
  }

  render () {
    const {
      showContent
    } = this.props

    if (!showContent) {
      return this.renderTitleTab()
    }

    return (
      <WidgetWrapper>
          {this.renderTitle()}
          <Content>
            <WelcomeText>
              {getLocale('hnsTalkWidgetWelcomeTitle')}
            </WelcomeText>
            <ActionsWrapper>
              <CallButton onClick={this.shouldCreateCall}>
                {getLocale('hnsTalkWidgetStartButton')}
              </CallButton>
              <Privacy>
                <PrivacyLink
                  rel={'noopener'}
                  target={'_blank'}
                  href={'https://hns.com/privacy/browser/#hns-talk-learn'}
                >
                  {getLocale('hnsTalkWidgetAboutData')}
                </PrivacyLink>
              </Privacy>
            </ActionsWrapper>
          </Content>
      </WidgetWrapper>
    )
  }
}

export const HnsTalkWidget = createWidget(HnsTalk)
