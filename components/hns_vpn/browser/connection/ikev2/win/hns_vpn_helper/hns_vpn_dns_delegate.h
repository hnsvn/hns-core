// Copyright (c) 2022 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef HNS_COMPONENTS_HNS_VPN_BROWSER_CONNECTION_IKEV2_WIN_HNS_VPN_HELPER_HNS_VPN_DNS_DELEGATE_H_
#define HNS_COMPONENTS_HNS_VPN_BROWSER_CONNECTION_IKEV2_WIN_HNS_VPN_HELPER_HNS_VPN_DNS_DELEGATE_H_

namespace hns_vpn {

class HnsVpnDnsDelegate {
 public:
  virtual void SignalExit() = 0;
};

}  // namespace hns_vpn

#endif  // HNS_COMPONENTS_HNS_VPN_BROWSER_CONNECTION_IKEV2_WIN_HNS_VPN_HELPER_HNS_VPN_DNS_DELEGATE_H_
