// Copyright (c) 2022 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at https://mozilla.org/MPL/2.0/.
import * as React from 'react'

interface Props {
    allowedLocales: string[]
}

/**
 * Displays it's children to only users locale within a certain locale
 */
export function LocaleRestricted ({
    allowedLocales,
    children
}: React.PropsWithChildren<Props>) {
    return allowedLocales.includes(Intl.NumberFormat().resolvedOptions().locale)
        ? <>{children}</>
        : null
}
