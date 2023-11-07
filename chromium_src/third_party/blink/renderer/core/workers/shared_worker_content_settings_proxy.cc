/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "src/third_party/blink/renderer/core/workers/shared_worker_content_settings_proxy.cc"

namespace blink {

HnsFarblingLevel SharedWorkerContentSettingsProxy::GetHnsFarblingLevel() {
  uint8_t result = HnsFarblingLevel::OFF;
  GetService()->GetHnsFarblingLevel(&result);
  if (result == 0)
    return HnsFarblingLevel::BALANCED;
  else if (result == 1)
    return HnsFarblingLevel::OFF;
  return HnsFarblingLevel::MAXIMUM;
}

}  // namespace blink
