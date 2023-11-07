/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

import styled from 'styled-components'

import * as mixins from '../../shared/lib/css_mixins'

export const root = styled.div`
  background: var(--hns-palette-white);
  box-shadow: 0px 0px 16px rgba(99, 105, 110, 0.18);
  border-radius: 16px;
  padding: 9px 18px 20px;
  font-family: var(--hns-font-heading);

  strong {
    font-weight: 600;
  }

  .hns-theme-dark & {
    background: #17171e;
  }
`

export const header = styled.div`
  display: flex;
  justify-content: space-between;
`

export const date = styled.div`
  border-radius: 48px;
  border: solid 1px var(--hns-palette-neutral200);
  padding: 3px 14px;
  font-weight: 600;
  font-size: 12px;
  line-height: 18px;
  color: var(--hns-palette-neutral700);
  text-transform: uppercase;

  .icon {
    height: 14px;
    width: auto;
    vertical-align: middle;
    margin-bottom: 2px;
    margin-right: 8px;
    color: var(--hns-palette-neutral600);
  }

  .hns-theme-dark & {
    color: var(--hns-palette-grey400);
    border-color: var(--hns-palette-grey700);

    .icon {
      color: var(--hns-palette-grey500);
    }
  }
`

export const close = styled.div`
  text-align: right;
  padding: 5px;

  button {
    ${mixins.buttonReset}
    color: var(--hns-palette-grey500);
    cursor: pointer;

    &:active {
      color: var(--hns-palette-grey600);
    }

    .hns-theme-dark & {
      color: var(--hns-palette-grey600);
    }

    .hns-theme-dark &:active {
      color: var(--hns-palette-grey500);
    }
  }

  .icon {
    height: 12px;
    width: auto;
  }
`

const actionButtonMixin = `
  ${mixins.buttonReset}
  width: 100%;
  font-weight: 600;
  font-size: 13px;
  line-height: 20px;
  border-radius: 48px;
  padding: 10px;
  cursor: pointer;
`

export const content = styled.div`
  text-align: center;
  font-size: 14px;
  line-height: 22px;
`

export const title = styled.div`
  margin: 20px 0 9px;
  font-weight: 500;
  font-size: 18px;
  line-height: 20px;
  color: var(--hns-palette-neutral900);
  text-align: center;

  .hns-theme-dark & {
    color: var(--hns-palette-grey000);
  }

  .icon {
    height: 16px;
    width: auto;
    vertical-align: middle;
    margin-right: 5px;
    margin-bottom: 3px;
    color: var(--hns-palette-green500);
  }

  &.funding .icon {
    color: var(--hns-palette-green500);
  }

  &.information .icon {
    color: var(--hns-palette-blue500);
  }

  &.error .icon {
    color: var(--hns-palette-red500);
  }

  &.custom {
    font-size: 16px;
  }
`

export const body = styled.div`
  margin-top: 10px;
  color: var(--hns-palette-neutral700);

  .hns-theme-dark & {
    color: var(--hns-palette-grey400);
  }
`

export const action = styled.div`
  margin-top: 16px;
  text-align: center;

  button {
    ${actionButtonMixin}
    color: var(--hns-palette-white);
    background: var(--hns-color-brandBat);

    &:active {
      background: var(--hns-palette-blurple400);
    }

    .hns-theme-dark & {
      color: var(--hns-palette-grey000);
    }
  }
`
