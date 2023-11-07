/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_rewards/browser/android_util.h"

namespace android_util {

hns_rewards::mojom::ClientInfoPtr GetAndroidClientInfo() {
  auto info = hns_rewards::mojom::ClientInfo::New();
  info->platform = hns_rewards::mojom::Platform::ANDROID_R;
  info->os = hns_rewards::mojom::OperatingSystem::UNDEFINED;
  return info;
}

}  // namespace android_util
