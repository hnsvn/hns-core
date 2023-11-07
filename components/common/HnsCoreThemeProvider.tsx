// Copyright (c) 2019 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at https://mozilla.org/MPL/2.0/.
import * as React from 'react'
import { ThemeProvider } from 'styled-components'
import DefaultTheme from 'hns-ui/theme/hns-default'
import DefaultDarkTheme from 'hns-ui/theme/hns-dark'
import IHnsTheme from 'hns-ui/theme/theme-interface'

export type Props = {
  // `initialThemeType` is not used!
  initialThemeType?: any
  dark?: IHnsTheme
  light?: IHnsTheme
}

const darkModeMediaMatcher = window.matchMedia('(prefers-color-scheme: dark)')

export default function LightDarkThemeProvider (props: React.PropsWithChildren<Props>) {
  const [isDarkMode, setIsDarkMode] = React.useState(darkModeMediaMatcher.matches)

  React.useEffect(() => {
    const handleDarkModeChange = (e: MediaQueryListEvent) => {
      setIsDarkMode(e.matches)
    }

    const onVisibilityChange = () => {
      if (document.visibilityState === 'visible') {
        setIsDarkMode(darkModeMediaMatcher.matches)
      }
    }

    darkModeMediaMatcher.addEventListener('change', handleDarkModeChange)
    document.addEventListener('visibilitychange', onVisibilityChange)

    return () => {
      darkModeMediaMatcher.removeEventListener('change', handleDarkModeChange)
      document.removeEventListener('visibilitychange', onVisibilityChange)
    }
  }, [])

  const selectedTheme = isDarkMode
    ? props.dark || DefaultDarkTheme
    : props.light || DefaultTheme
  return (
    <ThemeProvider theme={selectedTheme}>
      {React.Children.only(props.children)}
    </ThemeProvider>
  )
}
