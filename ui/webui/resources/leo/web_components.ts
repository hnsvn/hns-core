// Copyright (c) 2023 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

// Import web components here. They will be available on the page
// as <leo-{component}></leo-{component}>.
import '@hns/leo/web-components/button'
import { setIconBasePath } from '@hns/leo/web-components/icon'
import iconsMeta from '@hns/leo/icons/meta'

setIconBasePath('//resources/hns-icons')

window['leoIcons'] = new Set(Object.keys(iconsMeta.icons))
