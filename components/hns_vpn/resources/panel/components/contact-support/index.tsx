// Copyright (c) 2022 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at https://mozilla.org/MPL/2.0/.

import * as React from 'react'
import { CaratStrongLeftIcon } from 'hns-ui/components/icons'
import isValidEmailAddress from '$web-common/isValidEmailAddress'
import { getLocale } from '$web-common/locale'
import Button from '$web-components/button'
import Select from '$web-components/select'
import TextInput, { Textarea } from '$web-components/input'
import Toggle from '$web-components/toggle'
import getPanelBrowserAPI, * as HnsVPN from '../../api/panel_browser_api'
import { ErrorLabel } from '../general'
import * as S from './style'

interface Props {
  onCloseContactSupport: () => void
}

interface ContactSupportInputFields {
  contactEmail: string
  problemSubject: string
  problemBody: string
}

interface ContactSupportToggleFields {
  shareHostname: boolean
  shareAppVersion: boolean
  shareOsVersion: boolean
}

type ContactSupportState = ContactSupportInputFields &
                            ContactSupportToggleFields

const defaultSupportState: ContactSupportState = {
  contactEmail: '',
  problemSubject: '',
  problemBody: '',
  shareHostname: true,
  shareAppVersion: true,
  shareOsVersion: true
}

type FormElement = HTMLInputElement | HTMLTextAreaElement | HTMLSelectElement
type BaseType = string | number | React.FormEvent<FormElement>

function ContactSupport (props: Props) {
  const [supportData, setSupportData] = React.useState<HnsVPN.SupportData>()
  const [formData, setFormData] = React.useState<ContactSupportState>(defaultSupportState)
  const [showErrors, setShowErrors] = React.useState(false)
  // Undefined for never sent, true for is sending, false for has completed
  const [isSubmitting, setIsSubmitting] = React.useState<boolean>()
  const [isRemoteSubmissionError, setRemoteSubmissionError] = React.useState<boolean>(false)

  // Get possible values to submit
  React.useEffect(() => {
    getPanelBrowserAPI().serviceHandler.getSupportData()
      .then(setSupportData)
  }, [])

  function getOnChangeField<T extends BaseType = BaseType> (key: keyof ContactSupportInputFields) {
    return function (e: T) {
      const value = (typeof e === 'string' || typeof e === 'number') ? e : e.currentTarget.value
      if (formData[key] === value) {
        return
      }
      setFormData(data => ({
        ...data,
        [key]: value
      }))
    }
  }

  function getOnChangeToggle (key: keyof ContactSupportToggleFields) {
    return function (isOn: boolean) {
      if (formData[key] === isOn) {
        return
      }
      setFormData(data => ({
        ...data,
        [key]: isOn
      }))
    }
  }

  const emailAddressIsValid = React.useMemo(
    () => isValidEmailAddress(formData.contactEmail),
    [formData.contactEmail]
  )

  const isValid = React.useMemo(() => {
    return (
      !!formData.problemBody &&
      !!formData.contactEmail &&
      emailAddressIsValid &&
      !!formData.problemSubject
    )
  }, [formData, emailAddressIsValid])

  // Reset error states when data changes
  React.useEffect(() => {
    setShowErrors(false)
  }, [formData])

  const handleSubmit = async () => {
    // Clear error about last submission
    setRemoteSubmissionError(false)
    // Handle submission when not valid, show user
    // which fields are required
    if (!isValid) {
      setShowErrors(true)
      return
    }
    // Handle is valid, submit data
    setIsSubmitting(true)
    const fullIssueBody = `Message: \n${formData.problemBody}\n\n` +
      (formData.shareOsVersion ? `OS: ${supportData?.osVersion}\n` : '') +
      (formData.shareAppVersion ? `App version: ${supportData?.appVersion}\n` : '') +
      (formData.shareHostname ? `Hostname: ${supportData?.hostname}\n` : '')

    const { success } = await getPanelBrowserAPI().serviceHandler.createSupportTicket(
      formData.contactEmail,
      formData.problemSubject,
      fullIssueBody
    )

    setIsSubmitting(false)
    setRemoteSubmissionError(!success)

    if (success) {
      props.onCloseContactSupport()
    }
  }

  const handlePrivacyPolicyClick = () => {
    getPanelBrowserAPI().panelHandler.openVpnUI('privacy')
  }

  // Setup any individual field error message. Inputs handle 'required'
  // internally automatically.
  const emailAddressErrorMessage = (showErrors && formData.contactEmail && !emailAddressIsValid)
    ? getLocale('hnsVpnSupportEmailNotValid')
    : undefined

  return (
    <S.Box>
      <S.PanelContent>
        <S.PanelHeader>
          <S.BackButton
            type='button'
            onClick={props.onCloseContactSupport}
            aria-label='Close support form'
          >
            <i><CaratStrongLeftIcon /></i>
            <span>{getLocale('hnsVpnContactSupport')}</span>
          </S.BackButton>
        </S.PanelHeader>

        <S.Form onSubmit={e => { e.preventDefault() }}>
          <TextInput
            label={getLocale('hnsVpnSupportEmail')}
            isRequired={true}
            isErrorAlwaysShown={showErrors}
            value={formData.contactEmail ?? ''}
            errorMessage={emailAddressErrorMessage}
            onChange={getOnChangeField('contactEmail')}
          />
          <label>
            {getLocale('hnsVpnSupportSubject')}
            <Select
              ariaLabel={getLocale('hnsVpnSupportSubject')}
              value={formData.problemSubject ?? ''}
              onChange={getOnChangeField('problemSubject')}
            >
              <option value="" disabled>{getLocale('hnsVpnSupportSubjectNotSet')}</option>
              <option value="otherConnectionProblems">{getLocale('hnsVpnSupportSubjectOtherConnectionProblem')}</option>
              <option value="noInternet">{getLocale('hnsVpnSupportSubjectNoInternet')}</option>
              <option value="slowConnection">{getLocale('hnsVpnSupportSubjectSlowConnection')}</option>
              <option value="websiteProblems">{getLocale('hnsVpnSupportSubjectWebsiteDoesntWork')}</option>
              <option value="other">{getLocale('hnsVpnSupportSubjectOther')}</option>
            </Select>
            {showErrors && formData.problemSubject?.length === 0 &&
            <ErrorLabel>
              {getLocale('hnsVpnSupportSubjectNotSet')}
            </ErrorLabel>
            }
          </label>
          <Textarea
            value={formData.problemBody}
            label={getLocale('hnsVpnSupportBody')}
            isRequired={true}
            isErrorAlwaysShown={showErrors}
            onChange={getOnChangeField('problemBody')}
          />
          <S.OptionalValues>
            <S.SectionDescription>
              {getLocale('hnsVpnSupportOptionalHeader')}
            </S.SectionDescription>
            <S.Notes>
              <p>
                {getLocale('hnsVpnSupportOptionalNotes')}
                { ' ' }
                <a href="#" onClick={handlePrivacyPolicyClick}>
                  {getLocale('hnsVpnSupportOptionalNotesPrivacyPolicy')}
                </a>.
              </p>
            </S.Notes>
            <S.OptionalValueLabel>
              <div className={'optionalValueTitle'}>
                <span className={'optionalValueTitleKey'}>
                  {getLocale('hnsVpnSupportOptionalVpnHostname')}
                </span> {supportData?.hostname}
              </div>
              <Toggle
                isOn={formData.shareHostname}
                size={'sm'}
                onChange={getOnChangeToggle('shareHostname')}
              />
            </S.OptionalValueLabel>
            <S.OptionalValueLabel>
              <div className={'optionalValueTitle'}>
                <span className={'optionalValueTitleKey'}>
                  {getLocale('hnsVpnSupportOptionalAppVersion')}
                </span> {supportData?.appVersion}
              </div>
              <Toggle
                isOn={formData.shareAppVersion}
                size={'sm'}
                onChange={getOnChangeToggle('shareAppVersion')}
              />
            </S.OptionalValueLabel>
            <S.OptionalValueLabel>
              <div className={'optionalValueTitle'}>
                <span className={'optionalValueTitleKey'}>
                  {getLocale('hnsVpnSupportOptionalOsVersion')}
                </span> {supportData?.osVersion}
              </div>
              <Toggle
                isOn={formData.shareOsVersion}
                size={'sm'}
                onChange={getOnChangeToggle('shareOsVersion')}
              />
            </S.OptionalValueLabel>
            <S.OptionalValueLabel>
              <div className={'optionalValueTitle'}>
                <span className={'optionalValueTitleKey'}>
                  {getLocale('hnsVpnSupportTimezone')}
                </span> {supportData?.timezone}
              </div>
            </S.OptionalValueLabel>
          </S.OptionalValues>
          <S.Notes>
            <p>{getLocale('hnsVpnSupportNotes')}</p>
          </S.Notes>
          {isRemoteSubmissionError &&
          <ErrorLabel>
            {getLocale('hnsVpnSupportTicketFailed')}
          </ErrorLabel>
          }
          <Button
            type={'submit'}
            isPrimary
            isCallToAction
            isLoading={isSubmitting}
            isDisabled={isSubmitting}
            onClick={handleSubmit}
          >
            {getLocale('hnsVpnSupportSubmit')}
          </Button>
        </S.Form>
      </S.PanelContent>
    </S.Box>
  )
}

export default ContactSupport
