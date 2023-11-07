/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_vpn/browser/connection/wireguard/win/hns_vpn_wireguard_connection_api.h"

#include <memory>
#include <tuple>

#include "hns/components/hns_vpn/browser/connection/wireguard/hns_vpn_wireguard_connection_api_base.h"
#include "hns/components/hns_vpn/common/win/utils.h"
#include "hns/components/hns_vpn/common/wireguard/win/service_details.h"
#include "hns/components/hns_vpn/common/wireguard/win/wireguard_utils_win.h"

namespace hns_vpn {

namespace {
// Timer to recheck the service launch after some time.
constexpr int kWireguardServiceRestartTimeoutSec = 5;
}  // namespace

using ConnectionState = mojom::ConnectionState;

std::unique_ptr<HnsVPNOSConnectionAPI> CreateHnsVPNWireguardConnectionAPI(
    scoped_refptr<network::SharedURLLoaderFactory> url_loader_factory,
    PrefService* local_prefs,
    version_info::Channel channel) {
  return std::make_unique<HnsVPNWireguardConnectionAPI>(url_loader_factory,
                                                          local_prefs);
}

HnsVPNWireguardConnectionAPI::HnsVPNWireguardConnectionAPI(
    scoped_refptr<network::SharedURLLoaderFactory> url_loader_factory,
    PrefService* local_prefs)
    : HnsVPNWireguardConnectionAPIBase(url_loader_factory, local_prefs) {}

HnsVPNWireguardConnectionAPI::~HnsVPNWireguardConnectionAPI() {}

void HnsVPNWireguardConnectionAPI::Disconnect() {
  if (GetConnectionState() == ConnectionState::DISCONNECTED) {
    VLOG(2) << __func__ << " : already disconnected";
    return;
  }
  VLOG(2) << __func__ << " : Start stopping the service";
  UpdateAndNotifyConnectionStateChange(ConnectionState::DISCONNECTING);

  hns_vpn::wireguard::DisableHnsVpnWireguardService(
      base::BindOnce(&HnsVPNWireguardConnectionAPI::OnDisconnected,
                     weak_factory_.GetWeakPtr()));
}

void HnsVPNWireguardConnectionAPI::CheckConnection() {
  auto state = IsWindowsServiceRunning(
                   hns_vpn::GetHnsVpnWireguardTunnelServiceName())
                   ? ConnectionState::CONNECTED
                   : ConnectionState::DISCONNECTED;
  UpdateAndNotifyConnectionStateChange(state);
}

void HnsVPNWireguardConnectionAPI::RequestNewProfileCredentials() {
  hns_vpn::wireguard::WireguardGenerateKeypair(base::BindOnce(
      &HnsVPNWireguardConnectionAPI::OnWireguardKeypairGenerated,
      weak_factory_.GetWeakPtr()));
}

void HnsVPNWireguardConnectionAPI::PlatformConnectImpl(
    const wireguard::WireguardProfileCredentials& credentials) {
  auto vpn_server_hostname = GetHostname();
  auto config = hns_vpn::wireguard::CreateWireguardConfig(
      credentials.client_private_key, credentials.server_public_key,
      vpn_server_hostname, credentials.mapped_ip4_address);
  if (!config.has_value()) {
    VLOG(1) << __func__ << " : failed to get correct credentials";
    UpdateAndNotifyConnectionStateChange(ConnectionState::CONNECT_FAILED);
    return;
  }
  hns_vpn::wireguard::EnableHnsVpnWireguardService(
      config.value(),
      base::BindOnce(
          &HnsVPNWireguardConnectionAPI::OnWireguardServiceLaunched,
          weak_factory_.GetWeakPtr()));
}

void HnsVPNWireguardConnectionAPI::OnServiceStopped(int mask) {
  // Postpone check because the service can be restarted by the system due to
  // configured failure actions.
  base::SequencedTaskRunner::GetCurrentDefault()->PostDelayedTask(
      FROM_HERE,
      base::BindOnce(&HnsVPNWireguardConnectionAPI::CheckConnection,
                     weak_factory_.GetWeakPtr()),
      base::Seconds(kWireguardServiceRestartTimeoutSec));
  ResetServiceWatcher();
}

void HnsVPNWireguardConnectionAPI::RunServiceWatcher() {
  if (service_watcher_ && service_watcher_->IsWatching()) {
    return;
  }
  service_watcher_.reset(new hns::ServiceWatcher());
  if (!service_watcher_->Subscribe(
          hns_vpn::GetHnsVpnWireguardTunnelServiceName(),
          SERVICE_NOTIFY_STOPPED,
          base::BindRepeating(&HnsVPNWireguardConnectionAPI::OnServiceStopped,
                              weak_factory_.GetWeakPtr()))) {
    VLOG(1) << "Unable to set service watcher";
  }
}

void HnsVPNWireguardConnectionAPI::ResetServiceWatcher() {
  if (service_watcher_) {
    service_watcher_.reset();
  }
}

void HnsVPNWireguardConnectionAPI::OnWireguardServiceLaunched(bool success) {
  UpdateAndNotifyConnectionStateChange(
      success ? ConnectionState::CONNECTED : ConnectionState::CONNECT_FAILED);
}

void HnsVPNWireguardConnectionAPI::OnConnectionStateChanged(
    mojom::ConnectionState state) {
  HnsVPNWireguardConnectionAPIBase::OnConnectionStateChanged(state);
  if (state == ConnectionState::CONNECTED) {
    RunServiceWatcher();
    return;
  }
  ResetServiceWatcher();
}

}  // namespace hns_vpn
