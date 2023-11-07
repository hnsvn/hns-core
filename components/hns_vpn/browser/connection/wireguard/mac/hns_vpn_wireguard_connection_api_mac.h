/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_VPN_BROWSER_CONNECTION_WIREGUARD_MAC_HNS_VPN_WIREGUARD_CONNECTION_API_MAC_H_
#define HNS_COMPONENTS_HNS_VPN_BROWSER_CONNECTION_WIREGUARD_MAC_HNS_VPN_WIREGUARD_CONNECTION_API_MAC_H_

#include "base/memory/scoped_refptr.h"
#include "hns/components/hns_vpn/browser/connection/wireguard/hns_vpn_wireguard_connection_api_base.h"
#include "hns/components/hns_vpn/browser/connection/wireguard/credentials/hns_vpn_wireguard_profile_credentials.h"
#include "services/network/public/cpp/shared_url_loader_factory.h"

namespace hns_vpn {

class WireguardOSConnectionAPIMac : public HnsVPNWireguardConnectionAPIBase {
 public:
  WireguardOSConnectionAPIMac(
      scoped_refptr<network::SharedURLLoaderFactory> url_loader_factory,
      PrefService* local_prefs);

  WireguardOSConnectionAPIMac(const WireguardOSConnectionAPIMac&) = delete;
  WireguardOSConnectionAPIMac& operator=(const WireguardOSConnectionAPIMac&) =
      delete;
  ~WireguardOSConnectionAPIMac() override;

 private:
  // HnsVPNOSConnectionAPI
  void Disconnect() override;
  void CheckConnection() override;

  // HnsVPNWireguardConnectionAPIBase overrides:
  void RequestNewProfileCredentials() override;
  void PlatformConnectImpl(
      const wireguard::WireguardProfileCredentials& credentials) override;
};

}  // namespace hns_vpn

#endif  // HNS_COMPONENTS_HNS_VPN_BROWSER_CONNECTION_WIREGUARD_MAC_HNS_VPN_WIREGUARD_CONNECTION_API_MAC_H_
