// Copyright (c) 2021 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at https://mozilla.org/MPL/2.0/.
import styled, * as Styled from 'styled-components'
import IHnsTheme from 'hns-ui/theme/theme-interface'
import IWelcomeTheme from './welcome-theme'

type Theme = IHnsTheme & IWelcomeTheme

export default styled as unknown as Styled.ThemedStyledInterface<Theme>
export const css = Styled.css as Styled.ThemedCssFunction<Theme>
