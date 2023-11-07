/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_VERSION_INFO_VERSION_INFO_H_
#define HNS_COMPONENTS_VERSION_INFO_VERSION_INFO_H_

#include <string>

#include "hns/components/version_info/version_info_values.h"

namespace version_info {

std::string GetHnsVersionWithoutChromiumMajorVersion();

constexpr std::string GetHnsVersionNumberForDisplay() {
  return constants::kHnsVersionNumberForDisplay;
}

std::string GetHnsChromiumVersionNumber();

}  // namespace version_info

#endif  // HNS_COMPONENTS_VERSION_INFO_VERSION_INFO_H_
