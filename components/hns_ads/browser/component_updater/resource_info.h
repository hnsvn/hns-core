/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_BROWSER_COMPONENT_UPDATER_RESOURCE_INFO_H_
#define HNS_COMPONENTS_HNS_ADS_BROWSER_COMPONENT_UPDATER_RESOURCE_INFO_H_

#include <cstdint>
#include <string>

#include "base/files/file_path.h"

namespace hns_ads {

struct ResourceInfo {
  std::string id;
  uint16_t version;
  base::FilePath path;
};

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_BROWSER_COMPONENT_UPDATER_RESOURCE_INFO_H_
