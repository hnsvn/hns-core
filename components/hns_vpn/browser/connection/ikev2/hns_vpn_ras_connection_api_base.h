
/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_VPN_BROWSER_CONNECTION_IKEV2_HNS_VPN_RAS_CONNECTION_API_BASE_H_
#define HNS_COMPONENTS_HNS_VPN_BROWSER_CONNECTION_IKEV2_HNS_VPN_RAS_CONNECTION_API_BASE_H_

#include <memory>
#include <string>

#include "base/gtest_prod_util.h"
#include "base/memory/raw_ptr.h"
#include "base/memory/scoped_refptr.h"
#include "base/observer_list.h"
#include "base/observer_list_types.h"
#include "base/values.h"
#include "hns/components/hns_vpn/browser/connection/hns_vpn_connection_info.h"
#include "hns/components/hns_vpn/browser/connection/hns_vpn_os_connection_api.h"
#include "hns/components/hns_vpn/browser/connection/hns_vpn_region_data_manager.h"
#include "hns/components/hns_vpn/common/mojom/hns_vpn.mojom.h"
#include "net/base/network_change_notifier.h"
#include "services/network/public/cpp/shared_url_loader_factory.h"
#include "third_party/abseil-cpp/absl/types/optional.h"

class PrefService;

namespace hns_vpn {

class HnsVpnAPIRequest;

class HnsVPNOSConnectionAPIBase : public HnsVPNOSConnectionAPI {
 public:
  HnsVPNOSConnectionAPIBase(const HnsVPNOSConnectionAPIBase&) = delete;
  HnsVPNOSConnectionAPIBase& operator=(const HnsVPNOSConnectionAPIBase&) =
      delete;

  const HnsVPNConnectionInfo& connection_info() const;
  bool IsInProgress() const;

  // HnsVPNOSConnectionAPI
  void Connect() override;
  void Disconnect() override;
  void CheckConnection() override;
  void UpdateAndNotifyConnectionStateChange(
      mojom::ConnectionState state) override;
  void SetSelectedRegion(const std::string& name) override;
  void FetchProfileCredentials() override;
  void OnNetworkChanged(
      net::NetworkChangeNotifier::ConnectionType type) override;

 protected:
  HnsVPNOSConnectionAPIBase(
      scoped_refptr<network::SharedURLLoaderFactory> url_loader_factory,
      PrefService* local_prefs,
      version_info::Channel channel);
  ~HnsVPNOSConnectionAPIBase() override;

  // Subclass should add platform dependent impls.
  virtual void CreateVPNConnectionImpl(const HnsVPNConnectionInfo& info) = 0;
  virtual void ConnectImpl(const std::string& name) = 0;
  virtual void DisconnectImpl(const std::string& name) = 0;
  virtual void CheckConnectionImpl(const std::string& name) = 0;
  virtual bool IsPlatformNetworkAvailable() = 0;

  // Subclass should call below callbacks whenever corresponding event happens.
  void OnCreated();
  void OnCreateFailed();
  void OnConnected();
  void OnIsConnecting();
  void OnConnectFailed();
  void OnDisconnected();
  void OnIsDisconnecting();
  bool MaybeReconnect();

  std::string target_vpn_entry_name() const { return target_vpn_entry_name_; }

 private:
  friend class HnsVPNRegionDataManager;
  friend class HnsVPNOSConnectionAPISim;
  friend class HnsVPNOSConnectionAPIUnitTest;
  friend class HnsVPNServiceTest;

  FRIEND_TEST_ALL_PREFIXES(HnsVPNOSConnectionAPIUnitTest, NeedsConnectTest);
  FRIEND_TEST_ALL_PREFIXES(HnsVPNOSConnectionAPIUnitTest,
                           CreateOSVPNEntryWithValidInfoWhenConnectTest);
  FRIEND_TEST_ALL_PREFIXES(HnsVPNOSConnectionAPIUnitTest,
                           CreateOSVPNEntryWithInvalidInfoTest);
  FRIEND_TEST_ALL_PREFIXES(HnsVPNOSConnectionAPIUnitTest,
                           CheckConnectionStateAfterNetworkStateChanged);
  FRIEND_TEST_ALL_PREFIXES(HnsVPNOSConnectionAPIUnitTest, HostnamesTest);
  FRIEND_TEST_ALL_PREFIXES(HnsVPNOSConnectionAPIUnitTest,
                           CancelConnectingTest);
  FRIEND_TEST_ALL_PREFIXES(HnsVPNOSConnectionAPIUnitTest, ConnectionInfoTest);
  FRIEND_TEST_ALL_PREFIXES(HnsVPNOSConnectionAPIUnitTest,
                           IgnoreDisconnectedStateWhileConnecting);
  FRIEND_TEST_ALL_PREFIXES(HnsVPNOSConnectionAPIUnitTest,
                           ClearLastConnectionErrorWhenNewConnectionStart);
  FRIEND_TEST_ALL_PREFIXES(HnsVPNOSConnectionAPIUnitTest, ConnectionInfoTest);

  void ResetConnectionInfo();
  void CreateVPNConnection();
  void OnGetProfileCredentials(const std::string& profile_credential,
                               bool success);

  void SetPreventCreationForTesting(bool value);

  bool cancel_connecting_ = false;
  bool needs_connect_ = false;
  bool prevent_creation_ = false;
  std::string target_vpn_entry_name_;
  HnsVPNConnectionInfo connection_info_;
};

}  // namespace hns_vpn

#endif  // HNS_COMPONENTS_HNS_VPN_BROWSER_CONNECTION_IKEV2_HNS_VPN_RAS_CONNECTION_API_BASE_H_
