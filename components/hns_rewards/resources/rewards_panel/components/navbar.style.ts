/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

import styled from 'styled-components'

export const root = styled.div`
  background: var(--hns-palette-white);
  box-shadow: 0px 4px 16px rgba(27, 29, 47, 0.08);
  border-radius: 16px;
  color: var(--hns-palette-neutral600);
  font-size: 14px;
  line-height: 18px;
  margin-top: 12px;
  padding: 22px 30px;
  display: flex;
  justify-content: flex-end;
  align-items: center;

  button {
    font-weight: inherit;
    border: none;
    background: none;
    margin: 0;
    padding: 0;
    cursor: pointer;

    &:hover {
      color: var(--hns-palette-blurple400);
    }

    &:active {
      color: var(--hns-color-brandBatInteracting);
    }

    &[disabled] {
      cursor: default;
      color: inherit;
    }
  }

  .icon {
    width: 20px;
    height: auto;
    vertical-align: middle;
    margin-bottom: 2px;
    margin-right: 2px;
  }

  .selected button {
    font-weight: 600;
    color: var(--hns-color-brandBatInteracting);
  }

  .hns-theme-dark & {
    background: #1E2029;
    color: #999EA2;

    button:hover {
      color: var(--hns-palette-grey100);
    }

    .selected button {
      color: var(--hns-palette-blurple400);
    }

    button[disabled] {
      color: inherit;
    }
  }
`

export const tip = styled.div`
  flex: 1 1 auto;
  text-align: center;

  .icon {
    width: 24px;
  }
`

export const summary = styled.div`
  padding: 0 24px;

  .icon {
    margin-bottom: 4px;
  }
`

export const settings = styled.div`
  border-left: solid 1px var(--hns-palette-neutral200);
  padding-left: 16px;
  height: 24px;

  .icon {
    margin: 1px 0 0;
  }

  .hns-theme-dark & {
    border-color: var(--hns-palette-grey800);
    color: #999EA2;
  }
`
