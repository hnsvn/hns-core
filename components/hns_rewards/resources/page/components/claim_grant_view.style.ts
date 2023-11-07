/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

import styled from 'styled-components'

import * as mixins from '../../shared/lib/css_mixins'

export const root = styled.div`
  font-family: var(--hns-font-heading);
  background: var(--hns-palette-white);
  box-shadow: 0px 0px 10px rgba(99, 105, 110, 0.1);
  border-radius: 8px;
  display: flex;
  gap: 10px;
  overflow: hidden;
`

export const amountInfo = styled.div`
  flex: 0 1 auto;
  margin: 10px 0;
  padding-left: 16px;
  font-weight: 600;
  font-size: 12px;
  line-height: 18px;
  color: var(--hns-palette-blue500);
  display: flex;
  flex-direction: column;
  align-items: center;
  justify-content: center;
  gap: 2px;
`

export const graphic = styled.div`
  background: rgba(51, 154, 240, 0.15);
  padding: 8px;
  border-radius: 50%;

  .icon {
    height: 16px;
    width: auto;
    display: block;
  }
`

export const amount = styled.div`
  display: flex;
  gap: 2px;
`

export const text = styled.div`
  flex: 1 1 auto;
  margin: 12px 0;
  display: flex;
  flex-direction: column;
  justify-content: center;
  gap: 4px;
  font-size: 11px;
  line-height: 18px;
  color: var(--hns-palette-neutral700);
`

export const title = styled.div`
  font-weight: 500;
  font-size: 14px;
  line-height: 20px;
  letter-spacing: 0.01em;
  color: var(--hns-palette-neutral900);
`

export const days = styled.span`
  font-weight: 600;
`

export const action = styled.div`
  flex: 0 1 auto;
  display: flex;

  button {
    ${mixins.buttonReset}
    padding: 24px;
    text-transform: uppercase;
    cursor: pointer;
    background: var(--hns-palette-blurple500);
    color: var(--hns-palette-white);
    font-weight: 600;
    font-size: 12px;
    line-height: 18px;

    &:hover {
      background:
        linear-gradient(rgba(15, 28, 45, .05), rgba(15, 28, 45, .1)),
        var(--hns-palette-blurple500);
    }

    &:active {
      background: var(--hns-palette-blurple400);
    }

    .icon {
      height: 20px;
      width: auto;
      vertical-align: middle;
    }

    &[disabled] {
      background: var(--hns-palette-blurple500) !important;
    }
  }
`
