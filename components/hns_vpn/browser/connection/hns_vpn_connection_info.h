/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_VPN_BROWSER_CONNECTION_HNS_VPN_CONNECTION_INFO_H_
#define HNS_COMPONENTS_HNS_VPN_BROWSER_CONNECTION_HNS_VPN_CONNECTION_INFO_H_

#include <string>

namespace hns_vpn {

class HnsVPNConnectionInfo {
 public:
  HnsVPNConnectionInfo();
  ~HnsVPNConnectionInfo();
  HnsVPNConnectionInfo(const HnsVPNConnectionInfo& info);
  HnsVPNConnectionInfo& operator=(const HnsVPNConnectionInfo& info);

  void Reset();
  bool IsValid() const;
  void SetConnectionInfo(const std::string& connection_name,
                         const std::string& hostname,
                         const std::string& username,
                         const std::string& password);

  std::string connection_name() const { return connection_name_; }
  std::string hostname() const { return hostname_; }
  std::string username() const { return username_; }
  std::string password() const { return password_; }

 private:
  std::string connection_name_;
  std::string hostname_;
  std::string username_;
  std::string password_;
};

}  // namespace hns_vpn

#endif  // HNS_COMPONENTS_HNS_VPN_BROWSER_CONNECTION_HNS_VPN_CONNECTION_INFO_H_
