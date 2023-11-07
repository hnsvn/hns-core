/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_vpn/browser/connection/ikev2/hns_vpn_ras_connection_api_sim.h"

#include <memory>

#include "base/logging.h"
#include "base/notreached.h"
#include "base/rand_util.h"
#include "base/task/sequenced_task_runner.h"
#include "base/time/time.h"
#include "hns/components/hns_vpn/browser/connection/hns_vpn_os_connection_api.h"
#include "components/version_info/channel.h"

namespace hns_vpn {

HnsVPNOSConnectionAPISim::HnsVPNOSConnectionAPISim(
    scoped_refptr<network::SharedURLLoaderFactory> url_loader_factory,
    PrefService* local_prefs)
    : HnsVPNOSConnectionAPIBase(url_loader_factory,
                                  local_prefs,
                                  version_info::Channel::DEFAULT) {}

HnsVPNOSConnectionAPISim::~HnsVPNOSConnectionAPISim() = default;

void HnsVPNOSConnectionAPISim::CreateVPNConnectionImpl(
    const HnsVPNConnectionInfo& info) {
  base::SequencedTaskRunner::GetCurrentDefault()->PostTask(
      FROM_HERE,
      base::BindOnce(&HnsVPNOSConnectionAPISim::OnCreated,
                     weak_factory_.GetWeakPtr(), info.connection_name(), true));
}

void HnsVPNOSConnectionAPISim::ConnectImpl(const std::string& name) {
  disconnect_requested_ = false;

  // Determine connection success randomly.
  const bool success = base::RandInt(0, 9) > 3;
  // Simulate connection success
  if (success) {
    base::SequencedTaskRunner::GetCurrentDefault()->PostTask(
        FROM_HERE, base::BindOnce(&HnsVPNOSConnectionAPISim::OnIsConnecting,
                                  weak_factory_.GetWeakPtr(), name));

    base::SequencedTaskRunner::GetCurrentDefault()->PostDelayedTask(
        FROM_HERE,
        base::BindOnce(&HnsVPNOSConnectionAPISim::OnConnected,
                       weak_factory_.GetWeakPtr(), name, true),
        base::Seconds(1));
    return;
  }

  // Simulate connection failure
  base::SequencedTaskRunner::GetCurrentDefault()->PostTask(
      FROM_HERE, base::BindOnce(&HnsVPNOSConnectionAPISim::OnIsConnecting,
                                weak_factory_.GetWeakPtr(), name));
  base::SequencedTaskRunner::GetCurrentDefault()->PostDelayedTask(
      FROM_HERE,
      base::BindOnce(&HnsVPNOSConnectionAPISim::OnConnected,
                     weak_factory_.GetWeakPtr(), name, false),
      base::Seconds(1));
}

void HnsVPNOSConnectionAPISim::DisconnectImpl(const std::string& name) {
  disconnect_requested_ = true;

  base::SequencedTaskRunner::GetCurrentDefault()->PostTask(
      FROM_HERE, base::BindOnce(&HnsVPNOSConnectionAPISim::OnIsDisconnecting,
                                weak_factory_.GetWeakPtr(), name));

  base::SequencedTaskRunner::GetCurrentDefault()->PostTask(
      FROM_HERE, base::BindOnce(&HnsVPNOSConnectionAPISim::OnDisconnected,
                                weak_factory_.GetWeakPtr(), name, true));
}

void HnsVPNOSConnectionAPISim::Connect() {
  HnsVPNOSConnectionAPIBase::Connect();
}

void HnsVPNOSConnectionAPISim::CheckConnectionImpl(const std::string& name) {
  check_connection_called_ = true;
}

void HnsVPNOSConnectionAPISim::OnCreated(const std::string& name,
                                           bool success) {
  if (!success) {
    return;
  }
  connection_created_ = true;
  HnsVPNOSConnectionAPIBase::OnCreated();
}

void HnsVPNOSConnectionAPISim::Disconnect() {
  HnsVPNOSConnectionAPIBase::Disconnect();
}

void HnsVPNOSConnectionAPISim::CheckConnection() {
  HnsVPNOSConnectionAPIBase::CheckConnection();
}

bool HnsVPNOSConnectionAPISim::IsConnectionCreated() const {
  return connection_created_;
}

bool HnsVPNOSConnectionAPISim::IsConnectionChecked() const {
  return check_connection_called_;
}

void HnsVPNOSConnectionAPISim::OnConnected(const std::string& name,
                                             bool success) {
  // Cancelling connecting request simulation.
  if (disconnect_requested_) {
    disconnect_requested_ = false;
    return;
  }

  success ? HnsVPNOSConnectionAPIBase::OnConnected()
          : HnsVPNOSConnectionAPIBase::OnConnectFailed();
}

void HnsVPNOSConnectionAPISim::OnIsConnecting(const std::string& name) {
  HnsVPNOSConnectionAPIBase::OnIsConnecting();
}

void HnsVPNOSConnectionAPISim::OnDisconnected(const std::string& name,
                                                bool success) {
  if (!success) {
    return;
  }

  HnsVPNOSConnectionAPIBase::OnDisconnected();
}

void HnsVPNOSConnectionAPISim::OnIsDisconnecting(const std::string& name) {
  HnsVPNOSConnectionAPIBase::OnIsDisconnecting();
}

void HnsVPNOSConnectionAPISim::SetNetworkAvailableForTesting(bool value) {
  network_available_ = value;
}

bool HnsVPNOSConnectionAPISim::IsPlatformNetworkAvailable() {
  if (network_available_.has_value()) {
    return network_available_.value();
  }
  return true;
}

}  // namespace hns_vpn
