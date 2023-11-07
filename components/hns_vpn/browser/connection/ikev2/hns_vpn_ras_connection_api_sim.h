/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_VPN_BROWSER_CONNECTION_IKEV2_HNS_VPN_RAS_CONNECTION_API_SIM_H_
#define HNS_COMPONENTS_HNS_VPN_BROWSER_CONNECTION_IKEV2_HNS_VPN_RAS_CONNECTION_API_SIM_H_

#include <string>

#include "base/gtest_prod_util.h"
#include "base/memory/weak_ptr.h"
#include "base/no_destructor.h"
#include "hns/components/hns_vpn/browser/connection/hns_vpn_connection_info.h"
#include "hns/components/hns_vpn/browser/connection/ikev2/hns_vpn_ras_connection_api_base.h"

namespace hns_vpn {

class HnsVPNOSConnectionAPISim : public HnsVPNOSConnectionAPIBase {
 public:
  HnsVPNOSConnectionAPISim(
      scoped_refptr<network::SharedURLLoaderFactory> url_loader_factory,
      PrefService* local_prefs);
  ~HnsVPNOSConnectionAPISim() override;

  HnsVPNOSConnectionAPISim(const HnsVPNOSConnectionAPISim&) = delete;
  HnsVPNOSConnectionAPISim& operator=(const HnsVPNOSConnectionAPISim&) =
      delete;

  bool IsConnectionCreated() const;
  bool IsConnectionChecked() const;
  void SetNetworkAvailableForTesting(bool value);

 protected:
  friend class base::NoDestructor<HnsVPNOSConnectionAPISim>;

  // HnsVPNOSConnectionAPI overrides:
  void Connect() override;
  void Disconnect() override;
  void CheckConnection() override;

  // HnsVPNOSConnectionAPIBase interfaces:
  void CreateVPNConnectionImpl(const HnsVPNConnectionInfo& info) override;
  void ConnectImpl(const std::string& name) override;
  void DisconnectImpl(const std::string& name) override;
  void CheckConnectionImpl(const std::string& name) override;
  bool IsPlatformNetworkAvailable() override;

 private:
  friend class HnsVPNServiceTest;

  FRIEND_TEST_ALL_PREFIXES(HnsVPNOSConnectionAPIUnitTest,
                           CreateOSVPNEntryWithValidInfoWhenConnectTest);
  FRIEND_TEST_ALL_PREFIXES(HnsVPNOSConnectionAPIUnitTest,
                           CreateOSVPNEntryWithInvalidInfoTest);
  FRIEND_TEST_ALL_PREFIXES(HnsVPNServiceTest,
                           ConnectionStateUpdateWithPurchasedStateTest);
  FRIEND_TEST_ALL_PREFIXES(HnsVPNServiceTest, ResetConnectionStateTest);
  FRIEND_TEST_ALL_PREFIXES(HnsVPNServiceTest, DisconnectedIfDisabledByPolicy);

  void OnCreated(const std::string& name, bool success);
  void OnConnected(const std::string& name, bool success);
  void OnIsConnecting(const std::string& name);
  void OnDisconnected(const std::string& name, bool success);
  void OnIsDisconnecting(const std::string& name);
  void OnRemoved(const std::string& name, bool success);

  bool disconnect_requested_ = false;
  bool connection_created_ = false;
  bool check_connection_called_ = false;

  absl::optional<bool> network_available_;
  base::WeakPtrFactory<HnsVPNOSConnectionAPISim> weak_factory_{this};
};

}  // namespace hns_vpn

#endif  // HNS_COMPONENTS_HNS_VPN_BROWSER_CONNECTION_IKEV2_HNS_VPN_RAS_CONNECTION_API_SIM_H_
