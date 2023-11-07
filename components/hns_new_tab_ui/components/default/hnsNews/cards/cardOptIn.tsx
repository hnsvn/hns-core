// Copyright (c) 2020 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

import * as React from 'react'
import { getLocale, getLocaleWithTag } from '../../../../../common/locale'
import * as Card from '../cardIntro'
import HnsNewsLogoUrl from '../hnsNewsLogo.svg'
import { CardButton, TertiaryButton } from '../default'

type Props = {
  onOptIn: () => unknown
  onDisable: () => unknown
}

const descriptionTwoTextParts = getLocaleWithTag('hnsNewsIntroDescriptionTwo')

export default function IntroCard (props: Props) {
  const introElementRef = React.useRef(null)
  return (
    <Card.Intro ref={introElementRef}>
      <Card.Image src={HnsNewsLogoUrl} />
      <Card.Title>{getLocale('hnsNewsIntroTitle')}</Card.Title>
      <div>
        <Card.Paragraph>
          {getLocale('hnsNewsIntroDescription')}
        </Card.Paragraph>
        <Card.Paragraph>
          {descriptionTwoTextParts.beforeTag}
          <a href={'https://hns.com/privacy/browser/'}>
            {descriptionTwoTextParts.duringTag}
          </a>
          {descriptionTwoTextParts.afterTag}
        </Card.Paragraph>
      </div>
      <CardButton onClick={props.onOptIn} isMainFocus={true}>
        {getLocale('hnsNewsOptInActionLabel')}
      </CardButton>
      <TertiaryButton onClick={props.onDisable}>
        {getLocale('hnsNewsOptOutActionLabel')}
      </TertiaryButton>
    </Card.Intro>
  )
}
