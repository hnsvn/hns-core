/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

import * as React from 'react'
import styled, { ThemeContext } from 'styled-components'
import hnsDefaultTheme from 'hns-ui/theme/hns-default'
import hnsDarkTheme from 'hns-ui/theme/hns-dark'

function createThemeRules (theme: any) {
  if (!theme) {
    return ''
  }

  let list = []

  for (const [key, value] of Object.entries(theme.color)) {
    list.push(`--hns-color-${key}: ${String(value)};`)
  }
  for (const [key, value] of Object.entries(theme.palette)) {
    list.push(`--hns-palette-${key}: ${String(value)};`)
  }
  for (const [key, value] of Object.entries(theme.fontFamily)) {
    list.push(`--hns-font-${key}: ${String(value)};`)
  }

  return list.join('\n')
}

const Wrapper = styled.div`
  ${createThemeRules(hnsDefaultTheme)}

  &.hns-theme-dark {
    ${createThemeRules(hnsDarkTheme)}
  }
`

function normalizeThemeName (name: string) {
  if (name.toLowerCase() === 'dark' || name === hnsDarkTheme.name) {
    return 'dark'
  }
  return 'default'
}

export function WithThemeVariables (props: { children: React.ReactNode }) {
  const styledComponentsTheme = React.useContext(ThemeContext) || {}
  const [themeName, setThemeName] = React.useState('')

  React.useEffect(() => {
    if (chrome && chrome.hnsTheme) {
      chrome.hnsTheme.getHnsThemeType(setThemeName)
      chrome.hnsTheme.onHnsThemeTypeChanged.addListener(setThemeName)
    }
  }, [])

  const currentTheme = normalizeThemeName(
    themeName || styledComponentsTheme.name || '')

  return (
    <Wrapper className={`hns-theme-${currentTheme}`}>
      {props.children}
    </Wrapper>
  )
}
