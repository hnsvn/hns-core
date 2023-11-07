/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

import styled from 'styled-components'

import greenCheckIcon from '../assets/green_check.svg'

export const root = styled.div`
  position: relative;
  font-family: var(--hns-font-heading);
  max-width: 880px;
  min-width: 820px;
  min-height: 545px;
  margin: 0 10px;
  background: var(--hns-palette-white);
  box-shadow: 0px 0px 16px rgba(0, 0, 0, 0.36);
  border-radius: 16px;
  display: flex;
  overflow: hidden;

  a {
    color: var(--hns-color-brandBat);
    text-decoration: underline;
  }

  .layout-narrow & {
    display: block;
    max-width: 375px;
    min-width: unset;
    min-height: unset;
    border-radius: 8px;
  }
`

export const close = styled.div`
  position: absolute;
  top: 28px;
  right: 28px;

  .layout-narrow & {
    top: 16px;
    right: 16px;
  }
`

export const leftPanel = styled.div`
  flex: 0 1 448px;
  padding: 94px 64px;

  .layout-narrow & {
    padding: 40px 24px 24px;
  }
`

export const rightPanel = styled.div`
  flex: 1 1 430px;
  background: var(--hns-palette-neutral000);

  .layout-narrow & {
    background: var(--hns-palette-white);
  }
`

export const panelHeader = styled.div`
  flex: 1;
  font-weight: 600;
  font-size: 20px;
  line-height: 30px;
  color: var(--hns-palette-black);

  .layout-narrow & {
    font-size: 18px;
    line-height: 26px;
  }
`

export const panelText = styled.div`
  flex: 1;
  margin-top: 8px;
  font-size: 14px;
  line-height: 20px;
  color: var(--hns-palette-neutral700);
`

export const panelListItem = styled.div`
  margin-top: 24px;
  color: var(--hns-palette-black);
  font-weight: 600;
  font-size: 14px;
  line-height: 20px;
  background: top 2px left no-repeat url(/${greenCheckIcon});
  padding-left: 23px;
`

export const connectWalletLeftPanel = styled.div`
  display: flex;
  flex-direction: column;
  height: 100%;
`

export const connectWalletContent = styled.div`
  flex: 1 0 auto;
`

export const connectWalletDisclaimer = styled.div`
  font-size: 12px;
  line-height: 18px;
  color: var(--hns-palette-neutral600);
`

export const providerButtons = styled.div`
  padding: 20px 40px 40px;
  height: 100%;
  display: flex;
  flex-direction: column;
  justify-content: center;
  gap: 16px;

  button {
    height: 79px;
    display: flex;
    align-items: center;
    background: var(--hns-palette-white);
    border: 2px solid var(--hns-palette-grey200);
    border-radius: 8px;
    font-weight: 600;
    font-size: 16px;
    line-height: 28px;
    color: var(--hns-palette-black);
    padding: 14px 20px;
    cursor: pointer;

    &:active {
      border-color: var(--hns-palette-grey300);
    }

    &.selected {
      border-color: var(--hns-color-brandBat);
    }

    &.disabled {
      background: var(--hns-palette-neutral000);
    }
  }

  .layout-narrow & {
    padding: 0 24px 24px;
  }
`

export const providerButtonGrid = styled.div`
  display: grid;
  width: 100%;

  grid-template-areas: "icon name caret";
  grid-template-columns: auto 1fr auto;

  .disabled & {
    grid-template-areas:
      "icon name"
      "icon message";
    grid-template-columns: auto 1fr;
    grid-template-rows: auto;
  }
`

export const providerButtonIcon = styled.div`
  align-self: center;
  display: flex;
  height: 52px;
  width: 52px;

  background: var(--hns-palette-neutral000);

  grid-area: icon;

  border-radius: 50%;

  .icon {
    height: 25px;
    margin: auto;
  }

  .disabled & {
    background: var(--hns-palette-white);

    --provider-icon-color: var(--hns-palette-neutral600);
  }
`

export const providerButtonName = styled.div`
  align-items: center;
  display: flex;
  padding-left: 14px;

  font-size: 16px;
  font-weight: 600;
  line-height: normal;
  text-align: left;

  color: var(--hns-palette-black);

  grid-area: name;

  .disabled & {
    align-items: end;

    color: var(--hns-palette-neutral700);
  }
`

export const providerButtonMessage = styled.div`
  align-items: start;
  display: flex;
  padding-left: 14px;

  font-size: 11px;
  font-weight: 400;
  line-height: normal;
  text-align: left;

  color: var(--hns-palette-neutral700);

  grid-area: message;
`

export const providerButtonCaret = styled.div`
  display: flex;

  grid-area: caret;

  .icon {
    height: 16px;
    margin: auto;

    fill: var(--hns-palette-blurple500);
    stroke: var(--hns-palette-blurple500);
  }
`

export const learnMoreLink = styled.div``
