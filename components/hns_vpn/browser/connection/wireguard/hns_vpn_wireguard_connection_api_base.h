/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_VPN_BROWSER_CONNECTION_WIREGUARD_HNS_VPN_WIREGUARD_CONNECTION_API_BASE_H_
#define HNS_COMPONENTS_HNS_VPN_BROWSER_CONNECTION_WIREGUARD_HNS_VPN_WIREGUARD_CONNECTION_API_BASE_H_

#include <string>

#include "hns/components/hns_vpn/browser/connection/hns_vpn_os_connection_api.h"
#include "hns/components/hns_vpn/browser/connection/wireguard/credentials/hns_vpn_wireguard_profile_credentials.h"
#include "hns/components/hns_vpn/common/wireguard/wireguard_utils.h"

class PrefService;

namespace hns_vpn {

class HnsVPNWireguardConnectionAPIBase
    : public HnsVPNOSConnectionAPI,
      public HnsVPNOSConnectionAPI::Observer {
 public:
  HnsVPNWireguardConnectionAPIBase(
      scoped_refptr<network::SharedURLLoaderFactory> url_loader_factory,
      PrefService* local_prefs);

  HnsVPNWireguardConnectionAPIBase(
      const HnsVPNWireguardConnectionAPIBase&) = delete;
  HnsVPNWireguardConnectionAPIBase& operator=(
      const HnsVPNWireguardConnectionAPIBase&) = delete;
  ~HnsVPNWireguardConnectionAPIBase() override;

  // HnsVPNOSConnectionAPI
  void FetchProfileCredentials() override;
  void SetSelectedRegion(const std::string& name) override;
  void Connect() override;

  // Platform dependent APIs.
  virtual void RequestNewProfileCredentials() = 0;
  virtual void PlatformConnectImpl(
      const wireguard::WireguardProfileCredentials& credentials) = 0;

  // HnsVPNOSConnectionAPI::Observer
  void OnConnectionStateChanged(mojom::ConnectionState state) override;

 protected:
  void OnDisconnected(bool success);
  void OnWireguardKeypairGenerated(
      hns_vpn::wireguard::WireguardKeyPair key_pair);
  void OnGetProfileCredentials(const std::string& client_private_key,
                               const std::string& profile_credential,
                               bool success);
  void OnVerifyCredentials(const std::string& result, bool success);

 private:
  void ResetConnectionInfo();

  base::WeakPtrFactory<HnsVPNWireguardConnectionAPIBase> weak_factory_{this};
};

}  // namespace hns_vpn

#endif  // HNS_COMPONENTS_HNS_VPN_BROWSER_CONNECTION_WIREGUARD_HNS_VPN_WIREGUARD_CONNECTION_API_BASE_H_
