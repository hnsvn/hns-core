// Copyright (c) 2021 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at https://mozilla.org/MPL/2.0/.

const path = require('path')

module.exports = {
  // Find files in the current build configurations /gen directory
  'gen' : process.env.ROOT_GEN_DIR,
  // Generated resources at this path are available at chrome://resources and
  // whilst webpack will still bundle, we keep the alias to the served path
  // to minimize knowledge of specific gen/ paths and easily allow us to not
  // bundle
  // them in the future for certain build configurationa, just like chromium.
  'chrome://resources/hns/fonts' : path.join(
                                       process.env.ROOT_GEN_DIR,
                                       'hns/ui/webui/resources/fonts'),
  'chrome://resources/hns' : path.join(
                                 process.env.ROOT_GEN_DIR,
                                 'hns/ui/webui/resources/tsc'),
  'chrome://resources' : path.join(
                           process.env.ROOT_GEN_DIR, 'ui/webui/resources/tsc'),
  // We import hns-ui direct from source and not from package repo, so we need
  // direct path to the src/ directory.
  'hns-ui' : path.resolve(__dirname, '../../node_modules/@hns/hns-ui'),
  // Force same styled-components module for hns-core and hns-ui
  // which ensure both repos code use the same singletons, e.g. ThemeContext.
  'styled-components' : path.resolve(
                          __dirname, '../../node_modules/styled-components'),
  // More helpful path for local web-components
  '$web-components' : path.resolve(__dirname, '../web-components'),
  // TODO(petemill): Rename 'hns/components/common' dir to
  // 'hns/components/web-common'
  '$web-common' : path.resolve(__dirname, '../common'),
}
