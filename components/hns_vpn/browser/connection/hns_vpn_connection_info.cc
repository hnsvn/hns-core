/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_vpn/browser/connection/hns_vpn_connection_info.h"

namespace hns_vpn {

HnsVPNConnectionInfo::HnsVPNConnectionInfo() = default;
HnsVPNConnectionInfo::~HnsVPNConnectionInfo() = default;

HnsVPNConnectionInfo::HnsVPNConnectionInfo(
    const HnsVPNConnectionInfo& info) = default;
HnsVPNConnectionInfo& HnsVPNConnectionInfo::operator=(
    const HnsVPNConnectionInfo& info) = default;

void HnsVPNConnectionInfo::Reset() {
  connection_name_.clear();
  hostname_.clear();
  username_.clear();
  password_.clear();
}

bool HnsVPNConnectionInfo::IsValid() const {
  // TODO(simonhong): Improve credentials validation.
  return !hostname_.empty() && !username_.empty() && !password_.empty();
}

void HnsVPNConnectionInfo::SetConnectionInfo(
    const std::string& connection_name,
    const std::string& hostname,
    const std::string& username,
    const std::string& password) {
  connection_name_ = connection_name;
  hostname_ = hostname;
  username_ = username;
  password_ = password;
}

}  // namespace hns_vpn
