// Copyright (c) 2020 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef HNS_CHROMIUM_SRC_COMPONENTS_SYNC_DEVICE_INFO_DEVICE_INFO_TRACKER_H_
#define HNS_CHROMIUM_SRC_COMPONENTS_SYNC_DEVICE_INFO_DEVICE_INFO_TRACKER_H_

#include "base/functional/callback.h"

namespace syncer {

class HnsDeviceInfo;

}  // namespace syncer

#define ForcePulseForTest                                                      \
  DeleteDeviceInfo(const std::string& client_id, base::OnceClosure callback) { \
  }                                                                            \
  virtual std::vector<std::unique_ptr<HnsDeviceInfo>>                        \
  GetAllHnsDeviceInfo() const = 0;                                           \
  virtual void ForcePulseForTest

#include "src/components/sync_device_info/device_info_tracker.h"  // IWYU pragma: export

#undef ForcePulseForTest

#endif  // HNS_CHROMIUM_SRC_COMPONENTS_SYNC_DEVICE_INFO_DEVICE_INFO_TRACKER_H_
