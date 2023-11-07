// Copyright (c) 2022 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at https://mozilla.org/MPL/2.0/.

import * as React from 'react'

import * as S from './style'
import LottiePlayer from '../lottie-player'
import { getLocale } from '$web-common/locale'
import DataContext from '../../state/context'
import { ViewType } from '../../state/component_types'

const LOTTIE_CHECK_MARK_ICON = 'data:application/json;base64,eyJ2IjoiNS43LjQiLCJmciI6MjcsImlwIjozNS4wMDAwMDYzMjgzOTk4LCJvcCI6OTIsInciOjI0MCwiaCI6MjQwLCJubSI6IkNvbXAgMiIsImRkZCI6MCwiYXNzZXRzIjpbXSwibGF5ZXJzIjpbeyJkZGQiOjAsImluZCI6MSwidHkiOjQsIm5tIjoiU2hhcGUgTGF5ZXIgMiIsInNyIjoxLCJrcyI6eyJvIjp7ImEiOjAsImsiOjEwMCwiaXgiOjExfSwiciI6eyJhIjowLCJrIjowLCJpeCI6MTB9LCJwIjp7ImEiOjAsImsiOlsxMjAsMTIwLDBdLCJpeCI6MiwibCI6Mn0sImEiOnsiYSI6MCwiayI6WzAsMCwwXSwiaXgiOjEsImwiOjJ9LCJzIjp7ImEiOjAsImsiOlsyMi4yMjIyMjIyMjIyMjIyMTgsMjIuMjIyMjIyMjIyMjIyMjE4LDEwMF0sIml4Ijo2LCJsIjoyfX0sImFvIjowLCJzaGFwZXMiOlt7InR5IjoiZ3IiLCJpdCI6W3siaW5kIjowLCJ0eSI6InNoIiwiaXgiOjEsImtzIjp7ImEiOjAsImsiOnsiaSI6W1swLDBdLFswLDBdLFswLDBdXSwibyI6W1swLDBdLFswLDBdLFswLDBdXSwidiI6W1stMjkwLC04N10sWy00OSwxNTNdLFsxOTcsLTk0XV0sImMiOmZhbHNlfSwiaXgiOjJ9LCJubSI6IlBhdGggMSIsIm1uIjoiQURCRSBWZWN0b3IgU2hhcGUgLSBHcm91cCIsImhkIjpmYWxzZX0seyJ0eSI6InRtIiwicyI6eyJhIjoxLCJrIjpbeyJpIjp7IngiOlswLjY2N10sInkiOlsxXX0sIm8iOnsieCI6WzAuMTY3XSwieSI6WzAuMTY3XX0sInQiOjUzLjY2NiwicyI6WzBdfSx7InQiOjU5LjMzMzc2MDcyODIxOTgsInMiOlszMV19XSwiaXgiOjF9LCJlIjp7ImEiOjEsImsiOlt7ImkiOnsieCI6WzAuNjY3XSwieSI6WzFdfSwibyI6eyJ4IjpbMC4xNjddLCJ5IjpbMC4xNjddfSwidCI6NTMuNjY2LCJzIjpbMF19LHsidCI6NjAuMDAwMDEwODQ4Njg1NCwicyI6WzEwMF19XSwiaXgiOjJ9LCJvIjp7ImEiOjAsImsiOjAsIml4IjozfSwibSI6MSwiaXgiOjIsIm5tIjoiVHJpbSBQYXRocyAxIiwibW4iOiJBREJFIFZlY3RvciBGaWx0ZXIgLSBUcmltIiwiaGQiOmZhbHNlfSx7InR5Ijoic3QiLCJjIjp7ImEiOjAsImsiOlsxLDEsMSwxXSwiaXgiOjN9LCJvIjp7ImEiOjAsImsiOjEwMCwiaXgiOjR9LCJ3Ijp7ImEiOjAsImsiOjY1LCJpeCI6NX0sImxjIjoyLCJsaiI6MiwiYm0iOjAsIm5tIjoiU3Ryb2tlIDEiLCJtbiI6IkFEQkUgVmVjdG9yIEdyYXBoaWMgLSBTdHJva2UiLCJoZCI6ZmFsc2V9LHsidHkiOiJ0ciIsInAiOnsiYSI6MCwiayI6WzAsMF0sIml4IjoyfSwiYSI6eyJhIjowLCJrIjpbMCwwXSwiaXgiOjF9LCJzIjp7ImEiOjAsImsiOlsxMDAsMTAwXSwiaXgiOjN9LCJyIjp7ImEiOjAsImsiOjAsIml4Ijo2fSwibyI6eyJhIjowLCJrIjoxMDAsIml4Ijo3fSwic2siOnsiYSI6MCwiayI6MCwiaXgiOjR9LCJzYSI6eyJhIjowLCJrIjowLCJpeCI6NX0sIm5tIjoiVHJhbnNmb3JtIn1dLCJubSI6IlNoYXBlIDEiLCJucCI6NCwiY2l4IjoyLCJibSI6MCwiaXgiOjEsIm1uIjoiQURCRSBWZWN0b3IgR3JvdXAiLCJoZCI6ZmFsc2V9XSwiaXAiOjAsIm9wIjoyNzAuMDAwMDQ4ODE5MDg0LCJzdCI6MCwiYm0iOjB9LHsiZGRkIjowLCJpbmQiOjIsInR5Ijo0LCJubSI6IlNoYXBlIExheWVyIDEiLCJzciI6MSwia3MiOnsibyI6eyJhIjowLCJrIjoxMDAsIml4IjoxMX0sInIiOnsiYSI6MCwiayI6MCwiaXgiOjEwfSwicCI6eyJhIjowLCJrIjpbMTIwLDEyMCwwXSwiaXgiOjIsImwiOjJ9LCJhIjp7ImEiOjAsImsiOlswLDAsMF0sIml4IjoxLCJsIjoyfSwicyI6eyJhIjowLCJrIjpbMjIuMjIyMjIyMjIyMjIyMjE4LDIyLjIyMjIyMjIyMjIyMjIxOCwxMDBdLCJpeCI6NiwibCI6Mn19LCJhbyI6MCwic2hhcGVzIjpbeyJ0eSI6ImdyIiwiaXQiOlt7ImQiOjEsInR5IjoiZWwiLCJzIjp7ImEiOjAsImsiOls3MTAuODEyLDcxMC44MTJdLCJpeCI6Mn0sInAiOnsiYSI6MCwiayI6WzAsMF0sIml4IjozfSwibm0iOiJFbGxpcHNlIFBhdGggMSIsIm1uIjoiQURCRSBWZWN0b3IgU2hhcGUgLSBFbGxpcHNlIiwiaGQiOmZhbHNlfSx7InR5Ijoic3QiLCJjIjp7ImEiOjAsImsiOlsxLDEsMSwxXSwiaXgiOjN9LCJvIjp7ImEiOjAsImsiOjEwMCwiaXgiOjR9LCJ3Ijp7ImEiOjAsImsiOjY1LCJpeCI6NX0sImxjIjoyLCJsaiI6MiwiYm0iOjAsIm5tIjoiU3Ryb2tlIDEiLCJtbiI6IkFEQkUgVmVjdG9yIEdyYXBoaWMgLSBTdHJva2UiLCJoZCI6ZmFsc2V9LHsidHkiOiJ0ciIsInAiOnsiYSI6MCwiayI6WzI2LjAwOCwyOS45M10sIml4IjoyfSwiYSI6eyJhIjowLCJrIjpbMCwwXSwiaXgiOjF9LCJzIjp7ImEiOjAsImsiOlsxMDAsMTAwXSwiaXgiOjN9LCJyIjp7ImEiOjAsImsiOjAsIml4Ijo2fSwibyI6eyJhIjowLCJrIjoxMDAsIml4Ijo3fSwic2siOnsiYSI6MCwiayI6MCwiaXgiOjR9LCJzYSI6eyJhIjowLCJrIjowLCJpeCI6NX0sIm5tIjoiVHJhbnNmb3JtIn1dLCJubSI6IkVsbGlwc2UgMSIsIm5wIjozLCJjaXgiOjIsImJtIjowLCJpeCI6MSwibW4iOiJBREJFIFZlY3RvciBHcm91cCIsImhkIjpmYWxzZX0seyJ0eSI6InRtIiwicyI6eyJhIjoxLCJrIjpbeyJpIjp7IngiOlswLjgzM10sInkiOlswLjgzM119LCJvIjp7IngiOlswLjMzM10sInkiOlswXX0sInQiOjM5LCJzIjpbMTAwXX0seyJ0Ijo1My42NjYyNTk3MDM0NzExLCJzIjpbMF19XSwiaXgiOjF9LCJlIjp7ImEiOjAsImsiOjEwMCwiaXgiOjJ9LCJvIjp7ImEiOjAsImsiOi03NywiaXgiOjN9LCJtIjoxLCJpeCI6Miwibm0iOiJUcmltIFBhdGhzIDEiLCJtbiI6IkFEQkUgVmVjdG9yIEZpbHRlciAtIFRyaW0iLCJoZCI6ZmFsc2V9XSwiaXAiOjAsIm9wIjoyNzAuMDAwMDQ4ODE5MDg0LCJzdCI6MCwiYm0iOjB9XSwibWFya2VycyI6W119'

function SetupComplete () {
  const { setViewType } = React.useContext(DataContext)

  const handleComplete = () => {
    setViewType(ViewType.HelpImprove)
  }

  return (
    <S.MainBox>
      <div className='icon-box'>
        <LottiePlayer
          animationUrl={LOTTIE_CHECK_MARK_ICON}
          autoPlay={true}
          singleLoop={true}
          onComplete={handleComplete}
        />
      </div>
      <div className="view-title">{getLocale('hnsWelcomeSetupCompleteLabel')}</div>
    </S.MainBox>
  )
}

export default SetupComplete