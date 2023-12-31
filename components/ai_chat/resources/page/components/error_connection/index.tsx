/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

import * as React from 'react'
import { getLocale } from '$web-common/locale'
import Icon from '@hns/leo/react/icon'
import Button from '@hns/leo/react/button'

import styles from './style.module.scss'

interface PromptAutoSuggestionProps {
  onRetry?: () => void
}

function ErrorConnection (props: PromptAutoSuggestionProps) {
  return (
    <div className={styles.box}>
      <Icon name="warning-circle-filled" className={styles.icon} />
      <div>
        <p>{getLocale('errorNetworkLabel')}</p>
        <div className={styles.actionsBox}>
          <Button onClick={props.onRetry}>
            {getLocale('retryButtonLabel')}
          </Button>
        </div>
      </div>
    </div>
  )
}

export default ErrorConnection
