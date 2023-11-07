/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_VPN_BROWSER_CONNECTION_HNS_VPN_OS_CONNECTION_API_H_
#define HNS_COMPONENTS_HNS_VPN_BROWSER_CONNECTION_HNS_VPN_OS_CONNECTION_API_H_

#include <memory>
#include <string>

#include "base/memory/raw_ptr.h"
#include "base/observer_list.h"
#include "base/observer_list_types.h"
#include "hns/components/hns_vpn/browser/api/hns_vpn_api_request.h"
#include "hns/components/hns_vpn/browser/connection/hns_vpn_region_data_manager.h"
#include "hns/components/hns_vpn/common/mojom/hns_vpn.mojom.h"
#include "net/base/network_change_notifier.h"

namespace network {
class SharedURLLoaderFactory;
}  // namespace network

namespace version_info {
enum class Channel;
}  // namespace version_info

class PrefService;

namespace hns_vpn {

class HnsVPNRegionDataManager;
struct Hostname;

// Interface for managing OS' vpn connection.
class HnsVPNOSConnectionAPI
    : public net::NetworkChangeNotifier::NetworkChangeObserver {
 public:
  ~HnsVPNOSConnectionAPI() override;

  class Observer : public base::CheckedObserver {
   public:
    virtual void OnConnectionStateChanged(mojom::ConnectionState state) = 0;
    // false when fetching region data is failed.
    virtual void OnRegionDataReady(bool success) {}
    virtual void OnSelectedRegionChanged(const std::string& region_name) {}

   protected:
    ~Observer() override = default;
  };

  // Shared APIs implementation between IKEv2/Wireguard connections.
  mojom::ConnectionState GetConnectionState() const;
  void AddObserver(Observer* observer);
  void RemoveObserver(Observer* observer);
  HnsVPNRegionDataManager& GetRegionDataManager();
  void ResetHostname();
  std::string GetHostname() const;
  void ResetConnectionState();
  // Returns user friendly error string if existed.
  // Otherwise returns empty.
  std::string GetLastConnectionError() const;
  void ToggleConnection();

  // Connection dependent APIs.
  virtual void Connect() = 0;
  virtual void Disconnect() = 0;
  virtual void CheckConnection() = 0;
  virtual void UpdateAndNotifyConnectionStateChange(
      mojom::ConnectionState state);
  virtual void SetSelectedRegion(const std::string& name) = 0;
  virtual void FetchProfileCredentials() = 0;

 protected:
  explicit HnsVPNOSConnectionAPI(
      scoped_refptr<network::SharedURLLoaderFactory> url_loader_factory,
      PrefService* local_prefs);

  std::string GetCurrentEnvironment() const;
  PrefService* local_prefs() { return local_prefs_; }
  // True when do quick cancel.
  bool QuickCancelIfPossible();
  void ResetAPIRequestInstance();
  HnsVpnAPIRequest* GetAPIRequest();
  void SetLastConnectionError(const std::string& error);
  void FetchHostnamesForRegion(const std::string& name);
  void OnFetchHostnames(const std::string& region,
                        const std::string& hostnames,
                        bool success);
  void ParseAndCacheHostnames(const std::string& region,
                              const base::Value::List& hostnames_value);
  // HnsVPNRegionDataManager callbacks
  // Notify it's ready when |regions_| is not empty.
  void NotifyRegionDataReady(bool ready) const;
  void NotifySelectedRegionChanged(const std::string& name) const;
  // net::NetworkChangeNotifier::NetworkChangeObserver
  void OnNetworkChanged(
      net::NetworkChangeNotifier::ConnectionType type) override;

 private:
  FRIEND_TEST_ALL_PREFIXES(HnsVPNOSConnectionAPIUnitTest, NeedsConnectTest);
  FRIEND_TEST_ALL_PREFIXES(HnsVPNOSConnectionAPIUnitTest, ConnectionInfoTest);
  FRIEND_TEST_ALL_PREFIXES(HnsVPNOSConnectionAPIUnitTest,
                           CancelConnectingTest);
  FRIEND_TEST_ALL_PREFIXES(HnsVPNServiceTest, ResetConnectionStateTest);
  FRIEND_TEST_ALL_PREFIXES(HnsVPNServiceTest,
                           ConnectionStateUpdateWithPurchasedStateTest);
  FRIEND_TEST_ALL_PREFIXES(HnsVPNServiceTest,
                           IsConnectedWithPurchasedStateTest);
  FRIEND_TEST_ALL_PREFIXES(HnsVPNServiceTest, DisconnectedIfDisabledByPolicy);
  FRIEND_TEST_ALL_PREFIXES(HnsVPNOSConnectionAPIUnitTest,
                           IgnoreDisconnectedStateWhileConnecting);
  FRIEND_TEST_ALL_PREFIXES(HnsVPNOSConnectionAPIUnitTest, HostnamesTest);
  FRIEND_TEST_ALL_PREFIXES(HnsVPNWireguardConnectionAPIUnitTest,
                           SetSelectedRegion);

  void SetConnectionStateForTesting(mojom::ConnectionState state);

  raw_ptr<PrefService> local_prefs_;

  std::unique_ptr<Hostname> hostname_;
  std::string last_connection_error_;
  // Only not null when there is active network request.
  // When network request is done, we reset this so we can know
  // whether we're waiting the response or not.
  // We can cancel connecting request quickly when fetching hostnames or
  // profile credentials is not yet finished by reset this.
  std::unique_ptr<HnsVpnAPIRequest> api_request_;
  scoped_refptr<network::SharedURLLoaderFactory> url_loader_factory_;
  mojom::ConnectionState connection_state_ =
      mojom::ConnectionState::DISCONNECTED;
  HnsVPNRegionDataManager region_data_manager_;
  base::ObserverList<Observer> observers_;
};

// Create platform specific api instance.
// NOTE: Don't call this method directly.
// Only HnsBrowserProcess need to use this method.
std::unique_ptr<HnsVPNOSConnectionAPI> CreateHnsVPNConnectionAPI(
    scoped_refptr<network::SharedURLLoaderFactory> url_loader_factory,
    PrefService* local_prefs,
    version_info::Channel channel);

}  // namespace hns_vpn

#endif  // HNS_COMPONENTS_HNS_VPN_BROWSER_CONNECTION_HNS_VPN_OS_CONNECTION_API_H_
