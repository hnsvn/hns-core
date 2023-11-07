/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_VPN_BROWSER_CONNECTION_IKEV2_WIN_HNS_VPN_RAS_CONNECTION_API_WIN_H_
#define HNS_COMPONENTS_HNS_VPN_BROWSER_CONNECTION_IKEV2_WIN_HNS_VPN_RAS_CONNECTION_API_WIN_H_

#include <string>

#include "base/memory/weak_ptr.h"
#include "hns/components/hns_vpn/browser/connection/hns_vpn_connection_info.h"
#include "hns/components/hns_vpn/browser/connection/ikev2/hns_vpn_ras_connection_api_base.h"
#include "hns/components/hns_vpn/browser/connection/ikev2/win/ras_utils.h"
#include "hns/components/hns_vpn/common/win/ras/ras_connection_observer.h"

namespace hns_vpn {
namespace ras {
enum class CheckConnectionResult;
}  // namespace ras

class HnsVPNOSConnectionAPIWin : public HnsVPNOSConnectionAPIBase,
                                   public ras::RasConnectionObserver {
 public:
  HnsVPNOSConnectionAPIWin(
      scoped_refptr<network::SharedURLLoaderFactory> url_loader_factory,
      PrefService* local_prefs,
      version_info::Channel channel);
  HnsVPNOSConnectionAPIWin(const HnsVPNOSConnectionAPIWin&) = delete;
  HnsVPNOSConnectionAPIWin& operator=(const HnsVPNOSConnectionAPIWin&) =
      delete;
  ~HnsVPNOSConnectionAPIWin() override;

 private:
  // HnsVPNOSConnectionAPIBase interfaces:
  void CreateVPNConnectionImpl(const HnsVPNConnectionInfo& info) override;
  void ConnectImpl(const std::string& name) override;
  void DisconnectImpl(const std::string& name) override;
  void CheckConnectionImpl(const std::string& name) override;
  bool IsPlatformNetworkAvailable() override;

  // ras::RasConnectionObserver overrides:
  void OnRasConnectionStateChanged() override;

  void OnCreated(const std::string& name,
                 const ras::RasOperationResult& result);
  void OnConnected(const ras::RasOperationResult& result);
  void OnDisconnected(const ras::RasOperationResult& result);
  void OnCheckConnection(const std::string& name,
                         ras::CheckConnectionResult result);

  base::WeakPtrFactory<HnsVPNOSConnectionAPIWin> weak_factory_{this};
};

}  // namespace hns_vpn

#endif  // HNS_COMPONENTS_HNS_VPN_BROWSER_CONNECTION_IKEV2_WIN_HNS_VPN_RAS_CONNECTION_API_WIN_H_
