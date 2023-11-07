// Copyright (c) 2022 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at https://mozilla.org/MPL/2.0/.

import * as React from 'react'

import * as S from './style'
import Button from '$web-components/button'
import { P3APhase, WelcomeBrowserProxyImpl } from '../../api/welcome_browser_proxy'
import { getLocale, splitStringForTag } from '$web-common/locale'

interface InputCheckboxProps {
  onChange?: (e: React.ChangeEvent<HTMLInputElement>) => void
  children: React.ReactNode
  id?: string
  isChecked: boolean
}

function InputCheckbox (props: InputCheckboxProps) {
  return (
    <label className="item">
      <input
        type="checkbox"
        checked={props.isChecked}
        onChange={props.onChange}
      />
      <div>
        {props.children}
      </div>
    </label>
  )
}

function HelpImprove () {
  const [isMetricsReportingEnabled, setMetricsReportingEnabled] = React.useState(true)
  const [isP3AEnabled, setP3AEnabled] = React.useState(true)

  const handleP3AChange = () => {
    setP3AEnabled(!isP3AEnabled)
  }

  const handleMetricsReportingChange = () => {
    setMetricsReportingEnabled(!isMetricsReportingEnabled)
  }

  const handleFinish = () => {
    WelcomeBrowserProxyImpl.getInstance().setP3AEnabled(isP3AEnabled)
    WelcomeBrowserProxyImpl.getInstance().setMetricsReportingEnabled(isMetricsReportingEnabled)
    WelcomeBrowserProxyImpl.getInstance().recordP3A(P3APhase.Finished)
    window.open('chrome://newtab', '_self')
  }

  const handleOpenSettingsPage = () => {
    WelcomeBrowserProxyImpl.getInstance().openSettingsPage()
  }

  const changeSettingsNote = splitStringForTag(getLocale('hnsWelcomeChangeSettingsNote'))
  const readPrivacyPolicy = splitStringForTag(getLocale('hnsWelcomePrivacyPolicyNote'))
  const diagnosticReportsLabel = splitStringForTag(getLocale('hnsWelcomeSendReportsLabel'))
  const hnsProductUsageDataLabel = splitStringForTag(getLocale('hnsWelcomeSendInsightsLabel'))

  return (
    <S.MainBox>
      <div className="view-header-box">
        <div className="view-details">
          <h1 className="view-title">{getLocale('hnsWelcomeHelpImproveHnsTitle')}</h1>
        </div>
      </div>
      <S.Grid>
        <div className="list">
          <InputCheckbox
            id="metrics"
            onChange={handleMetricsReportingChange}
            isChecked={isMetricsReportingEnabled}
          >
            {diagnosticReportsLabel.beforeTag}
            <a href="https://support.hns.com/hc/en-us/articles/360017905872-How-do-I-enable-or-disable-automatic-crash-reporting" target="_blank">
              {diagnosticReportsLabel.duringTag}
            </a>
            {diagnosticReportsLabel.afterTag}
          </InputCheckbox>
          <InputCheckbox
            id="p3a"
            onChange={handleP3AChange}
            isChecked={isP3AEnabled}
          >
            {hnsProductUsageDataLabel.beforeTag}
            <a href="https://support.hns.com/hc/en-us/articles/9140465918093-What-is-P3A-in-Hns-" target="_blank">
              {hnsProductUsageDataLabel.duringTag}
            </a>
            {hnsProductUsageDataLabel.afterTag}
          </InputCheckbox>
        </div>
      </S.Grid>
      <S.ActionBox>
        <div className="box-center">
          <Button
            isPrimary={true}
            onClick={handleFinish}
            scale="jumbo"
          >
            {getLocale('hnsWelcomeFinishButtonLabel')}
          </Button>
          <S.FootNote>
            {changeSettingsNote.beforeTag}
            <a href="hns://settings/privacy" onClick={handleOpenSettingsPage}>
              {changeSettingsNote.duringTag}
            </a>
            {changeSettingsNote.afterTag}
            <span>
              {readPrivacyPolicy.beforeTag}
              <a href="https://hns.com/privacy/browser" target="_blank">
                {readPrivacyPolicy.duringTag}
              </a>
              {readPrivacyPolicy.afterTag}
            </span>
          </S.FootNote>
        </div>
      </S.ActionBox>
    </S.MainBox>
  )
}

export default HelpImprove
