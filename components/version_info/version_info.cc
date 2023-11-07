/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/components/version_info/version_info.h"

#include "hns/components/version_info/version_info_values.h"

namespace version_info {

std::string GetHnsVersionWithoutChromiumMajorVersion() {
  return constants::kHnsBrowserVersion;
}

std::string GetHnsChromiumVersionNumber() {
  return constants::kHnsChromiumVersion;
}

}  // namespace version_info
