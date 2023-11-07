/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_VPN_BROWSER_CONNECTION_WIREGUARD_WIN_HNS_VPN_WIREGUARD_CONNECTION_API_H_
#define HNS_COMPONENTS_HNS_VPN_BROWSER_CONNECTION_WIREGUARD_WIN_HNS_VPN_WIREGUARD_CONNECTION_API_H_

#include <memory>

#include "base/memory/raw_ptr.h"
#include "base/memory/scoped_refptr.h"
#include "base/memory/weak_ptr.h"
#include "hns/components/hns_vpn/browser/connection/wireguard/hns_vpn_wireguard_connection_api_base.h"
#include "hns/components/hns_vpn/browser/connection/wireguard/credentials/hns_vpn_wireguard_profile_credentials.h"
#include "hns/components/hns_vpn/common/win/hns_windows_service_watcher.h"

class PrefService;

namespace hns_vpn {

class HnsVPNWireguardConnectionAPI
    : public HnsVPNWireguardConnectionAPIBase {
 public:
  HnsVPNWireguardConnectionAPI(
      scoped_refptr<network::SharedURLLoaderFactory> url_loader_factory,
      PrefService* local_prefs);

  HnsVPNWireguardConnectionAPI(const HnsVPNWireguardConnectionAPI&) =
      delete;
  HnsVPNWireguardConnectionAPI& operator=(
      const HnsVPNWireguardConnectionAPI&) = delete;
  ~HnsVPNWireguardConnectionAPI() override;

  // HnsVPNOSConnectionAPI
  void Disconnect() override;
  void CheckConnection() override;

  // HnsVPNOSConnectionAPI::Observer
  void OnConnectionStateChanged(mojom::ConnectionState state) override;

 protected:
  // HnsVPNWireguardConnectionAPIBase
  void RequestNewProfileCredentials() override;
  void PlatformConnectImpl(
      const wireguard::WireguardProfileCredentials& credentials) override;

 private:
  void RunServiceWatcher();
  void OnWireguardServiceLaunched(bool success);
  void OnServiceStopped(int mask);
  void ResetServiceWatcher();

  std::unique_ptr<hns::ServiceWatcher> service_watcher_;
  base::WeakPtrFactory<HnsVPNWireguardConnectionAPI> weak_factory_{this};
};

}  // namespace hns_vpn

#endif  // HNS_COMPONENTS_HNS_VPN_BROWSER_CONNECTION_WIREGUARD_WIN_HNS_VPN_WIREGUARD_CONNECTION_API_H_
