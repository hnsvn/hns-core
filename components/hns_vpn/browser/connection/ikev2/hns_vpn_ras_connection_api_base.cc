/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_vpn/browser/connection/ikev2/hns_vpn_ras_connection_api_base.h"

#include <utility>
#include <vector>

#include "base/check_is_test.h"
#include "base/functional/bind.h"
#include "base/json/json_reader.h"
#include "hns/components/hns_vpn/common/hns_vpn_data_types.h"
#include "hns/components/hns_vpn/common/hns_vpn_utils.h"

namespace hns_vpn {

using ConnectionState = mojom::ConnectionState;

HnsVPNOSConnectionAPIBase::HnsVPNOSConnectionAPIBase(
    scoped_refptr<network::SharedURLLoaderFactory> url_loader_factory,
    PrefService* local_prefs,
    version_info::Channel channel)
    : HnsVPNOSConnectionAPI(url_loader_factory, local_prefs),
      target_vpn_entry_name_(GetHnsVPNEntryName(channel)) {}

HnsVPNOSConnectionAPIBase::~HnsVPNOSConnectionAPIBase() = default;

void HnsVPNOSConnectionAPIBase::SetSelectedRegion(const std::string& name) {
  // TODO(simonhong): Can remove this when UI block region changes while
  // operation is in-progress.
  // Don't allow region change while operation is in-progress.
  auto connection_state = GetConnectionState();
  if (connection_state == ConnectionState::DISCONNECTING ||
      connection_state == ConnectionState::CONNECTING) {
    VLOG(2) << __func__ << ": Current state: " << connection_state
            << " : prevent changing selected region while previous operation "
               "is in-progress";
    // This is workaround to prevent UI changes seleted region.
    // Early return by notify again with current region name.
    NotifySelectedRegionChanged(GetRegionDataManager().GetSelectedRegion());
    return;
  }

  GetRegionDataManager().SetSelectedRegion(name);

  // As new selected region is used, |connection_info_| for previous selected
  // should be cleared.
  ResetConnectionInfo();
}

bool HnsVPNOSConnectionAPIBase::IsInProgress() const {
  return GetConnectionState() == ConnectionState::DISCONNECTING ||
         GetConnectionState() == ConnectionState::CONNECTING;
}

void HnsVPNOSConnectionAPIBase::CreateVPNConnection() {
  if (cancel_connecting_) {
    UpdateAndNotifyConnectionStateChange(ConnectionState::DISCONNECTED);
    cancel_connecting_ = false;
    return;
  }

  if (prevent_creation_) {
    CHECK_IS_TEST();
    return;
  }
  CreateVPNConnectionImpl(connection_info_);
}

void HnsVPNOSConnectionAPIBase::SetPreventCreationForTesting(bool value) {
  prevent_creation_ = value;
}

void HnsVPNOSConnectionAPIBase::Connect() {
  if (IsInProgress()) {
    VLOG(2) << __func__ << ": Current state: " << GetConnectionState()
            << " : prevent connecting while previous operation is in-progress";
    return;
  }

  // Ignore connect request while cancelling is in-progress.
  if (cancel_connecting_) {
    return;
  }

  // User can ask connect again when user want to change region.
  if (GetConnectionState() == ConnectionState::CONNECTED) {
    // Disconnect first and then create again to setup for new region.
    // Set needs_connect to connect again after disconnected.
    needs_connect_ = true;
    Disconnect();
    return;
  }

  VLOG(2) << __func__ << " : start connecting!";
  SetLastConnectionError(std::string());
  UpdateAndNotifyConnectionStateChange(ConnectionState::CONNECTING);

  if (connection_info_.IsValid()) {
    VLOG(2) << __func__
            << " : Create os vpn entry with cached connection_info.";
    CreateVPNConnectionImpl(connection_info_);
    return;
  }

  // If user doesn't select region explicitely, use default device region.
  std::string target_region_name = GetRegionDataManager().GetSelectedRegion();
  if (target_region_name.empty()) {
    target_region_name = GetRegionDataManager().GetDeviceRegion();
    VLOG(2) << __func__ << " : start connecting with valid default_region: "
            << target_region_name;
  }
  DCHECK(!target_region_name.empty());
  FetchHostnamesForRegion(target_region_name);
}

void HnsVPNOSConnectionAPIBase::Disconnect() {
  if (GetConnectionState() == ConnectionState::DISCONNECTED) {
    VLOG(2) << __func__ << " : already disconnected";
    return;
  }

  if (GetConnectionState() == ConnectionState::DISCONNECTING) {
    VLOG(2) << __func__ << " : disconnecting in progress";
    return;
  }

  if (GetConnectionState() != ConnectionState::CONNECTING) {
    VLOG(2) << __func__ << " : start disconnecting!";
    UpdateAndNotifyConnectionStateChange(ConnectionState::DISCONNECTING);
    DisconnectImpl(target_vpn_entry_name_);
    return;
  }

  cancel_connecting_ = true;
  VLOG(2) << __func__ << " : Start cancelling connect request";
  UpdateAndNotifyConnectionStateChange(ConnectionState::DISCONNECTING);

  if (QuickCancelIfPossible()) {
    VLOG(2) << __func__ << " : Do quick cancel";
    UpdateAndNotifyConnectionStateChange(ConnectionState::DISCONNECTED);
    cancel_connecting_ = false;
  }
}

void HnsVPNOSConnectionAPIBase::CheckConnection() {
  CheckConnectionImpl(target_vpn_entry_name_);
}

void HnsVPNOSConnectionAPIBase::ResetConnectionInfo() {
  VLOG(2) << __func__;
  connection_info_.Reset();
}

void HnsVPNOSConnectionAPIBase::OnCreated() {
  VLOG(2) << __func__;

  if (cancel_connecting_) {
    UpdateAndNotifyConnectionStateChange(ConnectionState::DISCONNECTED);
    cancel_connecting_ = false;
    return;
  }

  // It's time to ask connecting to os after vpn entry is created.
  ConnectImpl(target_vpn_entry_name_);
}

void HnsVPNOSConnectionAPIBase::OnCreateFailed() {
  VLOG(2) << __func__;

  // Clear connecting cancel request.
  if (cancel_connecting_) {
    cancel_connecting_ = false;
  }

  UpdateAndNotifyConnectionStateChange(ConnectionState::CONNECT_NOT_ALLOWED);
}

void HnsVPNOSConnectionAPIBase::OnConnected() {
  VLOG(2) << __func__;

  if (cancel_connecting_) {
    // As connect is done, we don't need more for cancelling.
    // Just start normal Disconenct() process.
    cancel_connecting_ = false;
    DisconnectImpl(target_vpn_entry_name_);
    return;
  }

  UpdateAndNotifyConnectionStateChange(ConnectionState::CONNECTED);
}

void HnsVPNOSConnectionAPIBase::OnIsConnecting() {
  VLOG(2) << __func__;

  if (!cancel_connecting_) {
    UpdateAndNotifyConnectionStateChange(ConnectionState::CONNECTING);
  }
}

void HnsVPNOSConnectionAPIBase::OnConnectFailed() {
  cancel_connecting_ = false;

  // Clear previously used connection info if failed.
  connection_info_.Reset();

  UpdateAndNotifyConnectionStateChange(ConnectionState::CONNECT_FAILED);
}

bool HnsVPNOSConnectionAPIBase::MaybeReconnect() {
  VLOG(2) << __func__;

  if (!needs_connect_) {
    VLOG(2) << "Should be called only when reconnect expected";
    return false;
  }
  if (GetConnectionState() != ConnectionState::DISCONNECTED) {
    VLOG(2) << "For reconnection we expect DISCONNECTED status";
    return false;
  }
  if (IsPlatformNetworkAvailable()) {
    needs_connect_ = false;
    Connect();
    return true;
  }
  return false;
}

void HnsVPNOSConnectionAPIBase::OnNetworkChanged(
    net::NetworkChangeNotifier::ConnectionType type) {
  if (needs_connect_ && MaybeReconnect()) {
    VLOG(2) << "Network is live, reconnecting";
    return;
  }
  HnsVPNOSConnectionAPI::OnNetworkChanged(type);
}

void HnsVPNOSConnectionAPIBase::OnDisconnected() {
  UpdateAndNotifyConnectionStateChange(ConnectionState::DISCONNECTED);
  // Sometimes disconnected event happens before network state restored,
  // we postpone reconnection in this cases.
  if (needs_connect_ && !MaybeReconnect()) {
    VLOG(2) << "Network is down, will be reconnected when connection restored";
  }
}

void HnsVPNOSConnectionAPIBase::OnIsDisconnecting() {
  VLOG(2) << __func__;
  UpdateAndNotifyConnectionStateChange(ConnectionState::DISCONNECTING);
}

void HnsVPNOSConnectionAPIBase::UpdateAndNotifyConnectionStateChange(
    ConnectionState state) {
  // this is a simple state machine for handling connection state
  if (GetConnectionState() == state) {
    return;
  }

  // Ignore disconnected state while connecting is in-progress.
  // Network status can be changed during the vpn connection because
  // establishing vpn connection could make system network offline temporarily.
  // Whenever we get network status change, we check vpn connection state and
  // it could give disconnected vpn connection during that situation.
  // So, don't notify this disconnected state change while connecting because
  // it's temporal state.
  if (GetConnectionState() == ConnectionState::CONNECTING &&
      state == ConnectionState::DISCONNECTED && !cancel_connecting_) {
    VLOG(2) << __func__ << ": Ignore disconnected state while connecting";
    return;
  }
#if BUILDFLAG(IS_WIN)
  // On Windows, we could get disconnected state after connect failed.
  // To make connect failed state as a last state, ignore disconnected state.
  if (GetConnectionState() == ConnectionState::CONNECT_FAILED &&
      state == ConnectionState::DISCONNECTED) {
    VLOG(2) << __func__ << ": Ignore disconnected state after connect failed";
    return;
  }
#endif  // BUILDFLAG(IS_WIN)
  VLOG(2) << __func__ << " : changing from " << GetConnectionState() << " to "
          << state;

  HnsVPNOSConnectionAPI::UpdateAndNotifyConnectionStateChange(state);
}

void HnsVPNOSConnectionAPIBase::FetchProfileCredentials() {
  GetAPIRequest()->GetProfileCredentials(
      base::BindOnce(&HnsVPNOSConnectionAPIBase::OnGetProfileCredentials,
                     base::Unretained(this)),
      GetSubscriberCredential(local_prefs()), GetHostname());
}

void HnsVPNOSConnectionAPIBase::OnGetProfileCredentials(
    const std::string& profile_credential,
    bool success) {
  DCHECK(!cancel_connecting_);

  if (!success) {
    VLOG(2) << __func__ << " : failed to get profile credential";
    UpdateAndNotifyConnectionStateChange(ConnectionState::CONNECT_FAILED);
    return;
  }

  ResetAPIRequestInstance();

  VLOG(2) << __func__ << " : received profile credential";

  absl::optional<base::Value> value =
      base::JSONReader::Read(profile_credential);
  if (value && value->is_dict()) {
    constexpr char kUsernameKey[] = "eap-username";
    constexpr char kPasswordKey[] = "eap-password";
    const auto& dict = value->GetDict();
    const std::string* username = dict.FindString(kUsernameKey);
    const std::string* password = dict.FindString(kPasswordKey);
    if (!username || !password) {
      VLOG(2) << __func__ << " : it's invalid profile credential";
      UpdateAndNotifyConnectionStateChange(ConnectionState::CONNECT_FAILED);
      return;
    }

    connection_info_.SetConnectionInfo(target_vpn_entry_name_, GetHostname(),
                                       *username, *password);
    // Let's create os vpn entry with |connection_info_|.
    CreateVPNConnection();
    return;
  }

  VLOG(2) << __func__ << " : it's invalid profile credential";
  UpdateAndNotifyConnectionStateChange(ConnectionState::CONNECT_FAILED);
}

const HnsVPNConnectionInfo& HnsVPNOSConnectionAPIBase::connection_info()
    const {
  return connection_info_;
}

}  // namespace hns_vpn
