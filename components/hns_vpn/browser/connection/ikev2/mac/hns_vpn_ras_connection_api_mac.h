/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_VPN_BROWSER_CONNECTION_IKEV2_MAC_HNS_VPN_RAS_CONNECTION_API_MAC_H_
#define HNS_COMPONENTS_HNS_VPN_BROWSER_CONNECTION_IKEV2_MAC_HNS_VPN_RAS_CONNECTION_API_MAC_H_

#include <string>

#include "hns/components/hns_vpn/browser/connection/ikev2/hns_vpn_ras_connection_api_base.h"

namespace hns_vpn {

class HnsVPNOSConnectionAPIMac : public HnsVPNOSConnectionAPIBase {
 public:
  HnsVPNOSConnectionAPIMac(
      scoped_refptr<network::SharedURLLoaderFactory> url_loader_factory,
      PrefService* local_prefs,
      version_info::Channel channel);
  HnsVPNOSConnectionAPIMac(const HnsVPNOSConnectionAPIMac&) = delete;
  HnsVPNOSConnectionAPIMac& operator=(const HnsVPNOSConnectionAPIMac&) =
      delete;
  ~HnsVPNOSConnectionAPIMac() override;

 private:
  // HnsVPNOSConnectionAPIBase overrides:
  void CreateVPNConnectionImpl(const HnsVPNConnectionInfo& info) override;
  void ConnectImpl(const std::string& name) override;
  void DisconnectImpl(const std::string& name) override;
  void CheckConnectionImpl(const std::string& name) override;
  bool IsPlatformNetworkAvailable() override;
  void ObserveVPNConnectionChange();

  id vpn_observer_ = nil;
};

}  // namespace hns_vpn

#endif  // HNS_COMPONENTS_HNS_VPN_BROWSER_CONNECTION_IKEV2_MAC_HNS_VPN_RAS_CONNECTION_API_MAC_H_
