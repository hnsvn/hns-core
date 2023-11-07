/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_vpn/browser/connection/hns_vpn_os_connection_api.h"

#include <vector>

#include "base/check_is_test.h"
#include "base/feature_list.h"
#include "base/json/json_reader.h"
#include "base/memory/scoped_refptr.h"
#include "hns/components/hns_vpn/browser/api/hns_vpn_api_helper.h"
#include "hns/components/hns_vpn/common/hns_vpn_data_types.h"
#include "hns/components/hns_vpn/common/hns_vpn_utils.h"
#include "hns/components/hns_vpn/common/buildflags/buildflags.h"
#include "hns/components/hns_vpn/common/features.h"
#include "hns/components/hns_vpn/common/pref_names.h"
#include "build/build_config.h"
#include "components/prefs/pref_service.h"
#include "services/network/public/cpp/shared_url_loader_factory.h"

namespace hns_vpn {

std::unique_ptr<HnsVPNOSConnectionAPI> CreateHnsVPNIKEv2ConnectionAPI(
    scoped_refptr<network::SharedURLLoaderFactory> url_loader_factory,
    PrefService* local_prefs,
    version_info::Channel channel);

std::unique_ptr<HnsVPNOSConnectionAPI> CreateHnsVPNWireguardConnectionAPI(
    scoped_refptr<network::SharedURLLoaderFactory> url_loader_factory,
    PrefService* local_prefs,
    version_info::Channel channel);

std::unique_ptr<HnsVPNOSConnectionAPI> CreateHnsVPNConnectionAPI(
    scoped_refptr<network::SharedURLLoaderFactory> url_loader_factory,
    PrefService* local_prefs,
    version_info::Channel channel) {
#if BUILDFLAG(ENABLE_HNS_VPN_WIREGUARD)
  if (IsHnsVPNWireguardEnabled(local_prefs)) {
    return CreateHnsVPNWireguardConnectionAPI(url_loader_factory, local_prefs,
                                                channel);
  }
#endif
#if BUILDFLAG(IS_ANDROID)
  // Android doesn't use connection api.
  return nullptr;
#else
  return CreateHnsVPNIKEv2ConnectionAPI(url_loader_factory, local_prefs,
                                          channel);
#endif
}

HnsVPNOSConnectionAPI::HnsVPNOSConnectionAPI(
    scoped_refptr<network::SharedURLLoaderFactory> url_loader_factory,
    PrefService* local_prefs)
    : local_prefs_(local_prefs),
      url_loader_factory_(url_loader_factory),
      region_data_manager_(url_loader_factory, local_prefs) {
  DCHECK(url_loader_factory_);
  // Safe to use Unretained here because |region_data_manager_| is owned
  // instance.
  region_data_manager_.set_selected_region_changed_callback(
      base::BindRepeating(&HnsVPNOSConnectionAPI::NotifySelectedRegionChanged,
                          base::Unretained(this)));
  region_data_manager_.set_region_data_ready_callback(base::BindRepeating(
      &HnsVPNOSConnectionAPI::NotifyRegionDataReady, base::Unretained(this)));
  net::NetworkChangeNotifier::AddNetworkChangeObserver(this);
}

HnsVPNOSConnectionAPI::~HnsVPNOSConnectionAPI() {
  net::NetworkChangeNotifier::RemoveNetworkChangeObserver(this);
}

mojom::ConnectionState HnsVPNOSConnectionAPI::GetConnectionState() const {
  return connection_state_;
}

HnsVPNRegionDataManager& HnsVPNOSConnectionAPI::GetRegionDataManager() {
  return region_data_manager_;
}

void HnsVPNOSConnectionAPI::AddObserver(Observer* observer) {
  observers_.AddObserver(observer);
}

void HnsVPNOSConnectionAPI::RemoveObserver(Observer* observer) {
  observers_.RemoveObserver(observer);
}

void HnsVPNOSConnectionAPI::SetConnectionStateForTesting(
    mojom::ConnectionState state) {
  UpdateAndNotifyConnectionStateChange(state);
}

void HnsVPNOSConnectionAPI::NotifyRegionDataReady(bool ready) const {
  for (auto& obs : observers_) {
    obs.OnRegionDataReady(ready);
  }
}

void HnsVPNOSConnectionAPI::NotifySelectedRegionChanged(
    const std::string& name) const {
  for (auto& obs : observers_) {
    obs.OnSelectedRegionChanged(name);
  }
}

void HnsVPNOSConnectionAPI::OnNetworkChanged(
    net::NetworkChangeNotifier::ConnectionType type) {
  VLOG(1) << __func__ << " : " << type;
  CheckConnection();
}

HnsVpnAPIRequest* HnsVPNOSConnectionAPI::GetAPIRequest() {
  if (!url_loader_factory_) {
    CHECK_IS_TEST();
    return nullptr;
  }

  if (!api_request_) {
    api_request_ = std::make_unique<HnsVpnAPIRequest>(url_loader_factory_);
  }

  return api_request_.get();
}

void HnsVPNOSConnectionAPI::ResetHostname() {
  hostname_.reset();
}

void HnsVPNOSConnectionAPI::ResetConnectionState() {
  // Don't use UpdateAndNotifyConnectionStateChange() to update connection state
  // and set state directly because we have a logic to ignore disconnected state
  // when connect failed.
  connection_state_ = mojom::ConnectionState::DISCONNECTED;
  for (auto& obs : observers_) {
    obs.OnConnectionStateChanged(connection_state_);
  }
}

void HnsVPNOSConnectionAPI::UpdateAndNotifyConnectionStateChange(
    mojom::ConnectionState state) {
  // this is a simple state machine for handling connection state
  if (connection_state_ == state) {
    return;
  }

  connection_state_ = state;
  for (auto& obs : observers_) {
    obs.OnConnectionStateChanged(connection_state_);
  }
}

bool HnsVPNOSConnectionAPI::QuickCancelIfPossible() {
  if (!api_request_) {
    return false;
  }

  // We're waiting responce from vpn server.
  // Can do quick cancel in this situation by cancel that request.
  ResetAPIRequestInstance();
  return true;
}
std::string HnsVPNOSConnectionAPI::GetHostname() const {
  return hostname_ ? hostname_->hostname : "";
}

void HnsVPNOSConnectionAPI::ResetAPIRequestInstance() {
  api_request_.reset();
}

std::string HnsVPNOSConnectionAPI::GetLastConnectionError() const {
  return last_connection_error_;
}

void HnsVPNOSConnectionAPI::SetLastConnectionError(const std::string& error) {
  VLOG(2) << __func__ << " : " << error;
  last_connection_error_ = error;
}

void HnsVPNOSConnectionAPI::FetchHostnamesForRegion(const std::string& name) {
  // Hostname will be replaced with latest one.
  hostname_.reset();

  if (!GetAPIRequest()) {
    CHECK_IS_TEST();
    return;
  }

  // Unretained is safe here becasue this class owns request helper.
  GetAPIRequest()->GetHostnamesForRegion(
      base::BindOnce(&HnsVPNOSConnectionAPI::OnFetchHostnames,
                     base::Unretained(this), name),
      name);
}

void HnsVPNOSConnectionAPI::OnFetchHostnames(const std::string& region,
                                               const std::string& hostnames,
                                               bool success) {
  if (!success) {
    VLOG(2) << __func__ << " : failed to fetch hostnames for " << region;
    UpdateAndNotifyConnectionStateChange(
        mojom::ConnectionState::CONNECT_FAILED);
    return;
  }

  ResetAPIRequestInstance();

  absl::optional<base::Value> value = base::JSONReader::Read(hostnames);
  if (value && value->is_list()) {
    ParseAndCacheHostnames(region, value->GetList());
    return;
  }

  VLOG(2) << __func__ << " : failed to fetch hostnames for " << region;
  UpdateAndNotifyConnectionStateChange(mojom::ConnectionState::CONNECT_FAILED);
}

void HnsVPNOSConnectionAPI::ParseAndCacheHostnames(
    const std::string& region,
    const base::Value::List& hostnames_value) {
  std::vector<Hostname> hostnames = ParseHostnames(hostnames_value);

  if (hostnames.empty()) {
    VLOG(2) << __func__ << " : got empty hostnames list for " << region;
    UpdateAndNotifyConnectionStateChange(
        mojom::ConnectionState::CONNECT_FAILED);
    return;
  }

  hostname_ = PickBestHostname(hostnames);
  if (hostname_->hostname.empty()) {
    VLOG(2) << __func__ << " : got empty hostnames list for " << region;
    UpdateAndNotifyConnectionStateChange(
        mojom::ConnectionState::CONNECT_FAILED);
    return;
  }

  VLOG(2) << __func__ << " : Picked " << hostname_->hostname << ", "
          << hostname_->display_name << ", " << hostname_->is_offline << ", "
          << hostname_->capacity_score;

  if (!GetAPIRequest()) {
    CHECK_IS_TEST();
    return;
  }

  // Get profile credentials it to create OS VPN entry.
  VLOG(2) << __func__ << " : request profile credential:"
          << GetHnsVPNPaymentsEnv(GetCurrentEnvironment());
  FetchProfileCredentials();
}

std::string HnsVPNOSConnectionAPI::GetCurrentEnvironment() const {
  return local_prefs_->GetString(prefs::kHnsVPNEnvironment);
}

void HnsVPNOSConnectionAPI::ToggleConnection() {
  const bool can_disconnect =
      (GetConnectionState() == mojom::ConnectionState::CONNECTED ||
       GetConnectionState() == mojom::ConnectionState::CONNECTING);
  can_disconnect ? Disconnect() : Connect();
}

}  // namespace hns_vpn
