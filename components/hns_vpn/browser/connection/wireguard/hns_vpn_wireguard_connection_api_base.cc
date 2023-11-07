/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_vpn/browser/connection/wireguard/hns_vpn_wireguard_connection_api_base.h"

#include "hns/components/hns_vpn/common/hns_vpn_utils.h"
#include "hns/components/hns_vpn/common/mojom/hns_vpn.mojom.h"
#include "hns/components/hns_vpn/common/pref_names.h"
#include "components/prefs/pref_service.h"

namespace hns_vpn {

using ConnectionState = mojom::ConnectionState;

HnsVPNWireguardConnectionAPIBase::HnsVPNWireguardConnectionAPIBase(
    scoped_refptr<network::SharedURLLoaderFactory> url_loader_factory,
    PrefService* local_prefs)
    : HnsVPNOSConnectionAPI(url_loader_factory, local_prefs) {
  AddObserver(this);
}

HnsVPNWireguardConnectionAPIBase::~HnsVPNWireguardConnectionAPIBase() {
  RemoveObserver(this);
}

void HnsVPNWireguardConnectionAPIBase::SetSelectedRegion(
    const std::string& name) {
  GetRegionDataManager().SetSelectedRegion(name);
  ResetConnectionInfo();
}

void HnsVPNWireguardConnectionAPIBase::OnWireguardKeypairGenerated(
    hns_vpn::wireguard::WireguardKeyPair key_pair) {
  if (!key_pair.has_value()) {
    VLOG(1) << __func__ << " : failed to get keypair";
    UpdateAndNotifyConnectionStateChange(ConnectionState::CONNECT_FAILED);
    SetLastConnectionError("Failed to create keypair");
    return;
  }
  const auto [public_key, private_key] = key_pair.value();
  GetAPIRequest()->GetWireguardProfileCredentials(
      base::BindOnce(
          &HnsVPNWireguardConnectionAPIBase::OnGetProfileCredentials,
          base::Unretained(this), private_key),
      GetSubscriberCredential(local_prefs()), public_key, GetHostname());
}

void HnsVPNWireguardConnectionAPIBase::Connect() {
  VLOG(2) << __func__ << " : start connecting!";
  SetLastConnectionError(std::string());
  UpdateAndNotifyConnectionStateChange(ConnectionState::CONNECTING);

  // There's some fetched
  if (!GetHostname().empty()) {
    FetchProfileCredentials();
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

void HnsVPNWireguardConnectionAPIBase::OnGetProfileCredentials(
    const std::string& client_private_key,
    const std::string& profile_credentials,
    bool success) {
  if (!success) {
    VLOG(1) << __func__ << " : failed to get profile credential";
    UpdateAndNotifyConnectionStateChange(ConnectionState::CONNECT_FAILED);
    SetLastConnectionError("Failed to get profile credential");
    return;
  }
  auto parsed_credentials =
      wireguard::WireguardProfileCredentials::FromServerResponse(
          profile_credentials, client_private_key);
  if (!parsed_credentials.has_value()) {
    VLOG(1) << __func__ << " : failed to get correct credentials";
    UpdateAndNotifyConnectionStateChange(ConnectionState::CONNECT_FAILED);
    SetLastConnectionError("Failed to get correct credentials");
    return;
  }
  auto serialized = parsed_credentials->ToString();
  if (serialized.has_value()) {
    local_prefs()->SetString(prefs::kHnsVPNWireguardProfileCredentials,
                             serialized.value());
  }
  PlatformConnectImpl(parsed_credentials.value());
}

void HnsVPNWireguardConnectionAPIBase::FetchProfileCredentials() {
  if (!GetAPIRequest()) {
    return;
  }
  auto existing_credentials =
      wireguard::WireguardProfileCredentials::FromString(
          local_prefs()->GetString(
              prefs::kHnsVPNWireguardProfileCredentials));
  if (!existing_credentials.has_value()) {
    RequestNewProfileCredentials();
    return;
  }
  GetAPIRequest()->VerifyCredentials(
      base::BindOnce(&HnsVPNWireguardConnectionAPIBase::OnVerifyCredentials,
                     weak_factory_.GetWeakPtr()),
      GetHostname(), existing_credentials->client_id,
      GetSubscriberCredential(local_prefs()),
      existing_credentials->api_auth_token);
}

void HnsVPNWireguardConnectionAPIBase::ResetConnectionInfo() {
  VLOG(2) << __func__;
  ResetHostname();
  local_prefs()->SetString(prefs::kHnsVPNWireguardProfileCredentials,
                           std::string());
}

void HnsVPNWireguardConnectionAPIBase::OnVerifyCredentials(
    const std::string& result,
    bool success) {
  auto existing_credentials =
      wireguard::WireguardProfileCredentials::FromString(
          local_prefs()->GetString(
              prefs::kHnsVPNWireguardProfileCredentials));
  if (!success || !existing_credentials.has_value()) {
    VLOG(1) << __func__ << " : credentials verification failed ( " << result
            << " ), request new";
    RequestNewProfileCredentials();
    return;
  }
  PlatformConnectImpl(existing_credentials.value());
}

void HnsVPNWireguardConnectionAPIBase::OnConnectionStateChanged(
    mojom::ConnectionState state) {
  if (state == ConnectionState::CONNECT_FAILED) {
    ResetConnectionInfo();
  }
}

void HnsVPNWireguardConnectionAPIBase::OnDisconnected(bool success) {
  if (!success) {
    VLOG(1) << "Failed to stop wireguard tunnel service";
    SetLastConnectionError("Failed to stop wireguard tunnel service");
    UpdateAndNotifyConnectionStateChange(ConnectionState::CONNECTED);
    return;
  }

  UpdateAndNotifyConnectionStateChange(ConnectionState::DISCONNECTED);
}

}  // namespace hns_vpn
