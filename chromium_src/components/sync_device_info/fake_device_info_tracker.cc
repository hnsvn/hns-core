// Copyright (c) 2021 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

#include "hns/components/sync_device_info/hns_device_info.h"

#include "src/components/sync_device_info/fake_device_info_tracker.cc"

namespace syncer {

void FakeDeviceInfoTracker::DeleteDeviceInfo(const std::string& client_id,
                                             base::OnceClosure callback) {}

std::vector<std::unique_ptr<HnsDeviceInfo>>
FakeDeviceInfoTracker::GetAllHnsDeviceInfo() const {
  return std::vector<std::unique_ptr<HnsDeviceInfo>>();
}

}  // namespace syncer
