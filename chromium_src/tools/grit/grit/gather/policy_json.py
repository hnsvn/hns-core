# Copyright (c) 2022 The Hns Authors. All rights reserved.
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this file,
# You can obtain one at https://mozilla.org/MPL/2.0/.

import override_utils

@override_utils.override_method(PolicyJson)
def SetDefines(self, _, defines):
  self._config = {
    'build': 'hns',
    'app_name': 'Hns',
    'frame_name': 'Hns Frame',
    'os_name': 'Google Chrome OS'
  }
