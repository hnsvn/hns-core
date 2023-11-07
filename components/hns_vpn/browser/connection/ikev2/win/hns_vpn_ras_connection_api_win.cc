/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_vpn/browser/connection/ikev2/win/hns_vpn_ras_connection_api_win.h"

#include <windows.h>

#include <netlistmgr.h>  // For CLSID_NetworkListManager

#include <wrl/client.h>
#include <memory>

#include "base/logging.h"
#include "base/notreached.h"
#include "base/strings/utf_string_conversions.h"
#include "base/task/thread_pool.h"
#include "hns/components/hns_vpn/browser/connection/ikev2/win/ras_utils.h"
#include "hns/components/hns_vpn/common/hns_vpn_constants.h"
// Most of Windows implementations are based on Brian Clifton
// (brian@clifton.me)'s work (https://github.com/bsclifton/winvpntool).

using hns_vpn::ras::CheckConnectionResult;
using hns_vpn::ras::CreateEntry;
using hns_vpn::ras::RasOperationResult;
using hns_vpn::ras::RemoveEntry;

namespace hns_vpn {

namespace {

RasOperationResult ConnectEntry(const std::wstring& name) {
  return hns_vpn::ras::ConnectEntry(name);
}

RasOperationResult DisconnectEntry(const std::wstring& name) {
  return hns_vpn::ras::DisconnectEntry(name);
}

}  // namespace

std::unique_ptr<HnsVPNOSConnectionAPI> CreateHnsVPNIKEv2ConnectionAPI(
    scoped_refptr<network::SharedURLLoaderFactory> url_loader_factory,
    PrefService* local_prefs,
    version_info::Channel channel) {
  return std::make_unique<HnsVPNOSConnectionAPIWin>(url_loader_factory,
                                                      local_prefs, channel);
}

HnsVPNOSConnectionAPIWin::HnsVPNOSConnectionAPIWin(
    scoped_refptr<network::SharedURLLoaderFactory> url_loader_factory,
    PrefService* local_prefs,
    version_info::Channel channel)
    : HnsVPNOSConnectionAPIBase(url_loader_factory, local_prefs, channel) {
  StartRasConnectionChangeMonitoring();
}

HnsVPNOSConnectionAPIWin::~HnsVPNOSConnectionAPIWin() {}

void HnsVPNOSConnectionAPIWin::CreateVPNConnectionImpl(
    const HnsVPNConnectionInfo& info) {
  base::ThreadPool::PostTaskAndReplyWithResult(
      FROM_HERE, {base::MayBlock()}, base::BindOnce(&CreateEntry, info),
      base::BindOnce(&HnsVPNOSConnectionAPIWin::OnCreated,
                     weak_factory_.GetWeakPtr(), info.connection_name()));
}

void HnsVPNOSConnectionAPIWin::ConnectImpl(const std::string& name) {
  // Connection state update from this call will be done by monitoring.
  base::ThreadPool::PostTaskAndReplyWithResult(
      FROM_HERE, {base::MayBlock()},
      base::BindOnce(&ConnectEntry, base::UTF8ToWide(name)),
      base::BindOnce(&HnsVPNOSConnectionAPIWin::OnConnected,
                     weak_factory_.GetWeakPtr()));
}

void HnsVPNOSConnectionAPIWin::DisconnectImpl(const std::string& name) {
  // Connection state update from this call will be done by monitoring.
  base::ThreadPool::PostTaskAndReplyWithResult(
      FROM_HERE, {base::MayBlock()},
      base::BindOnce(&DisconnectEntry, base::UTF8ToWide(name)),
      base::BindOnce(&HnsVPNOSConnectionAPIWin::OnDisconnected,
                     weak_factory_.GetWeakPtr()));
}

void HnsVPNOSConnectionAPIWin::CheckConnectionImpl(const std::string& name) {
  base::ThreadPool::PostTaskAndReplyWithResult(
      FROM_HERE, {base::MayBlock()},
      base::BindOnce(&ras::CheckConnection, base::UTF8ToWide(name)),
      base::BindOnce(&HnsVPNOSConnectionAPIWin::OnCheckConnection,
                     weak_factory_.GetWeakPtr(), name));
}

void HnsVPNOSConnectionAPIWin::OnRasConnectionStateChanged() {
  DCHECK(!target_vpn_entry_name().empty());

  // Check connection state for HnsVPN entry again when connected or
  // disconnected events are arrived because we can get both event from any os
  // vpn entry. All other events are sent by our code at utils_win.cc.
  CheckConnectionImpl(target_vpn_entry_name());
}

void HnsVPNOSConnectionAPIWin::OnCheckConnection(
    const std::string& name,
    CheckConnectionResult result) {
  switch (result) {
    case CheckConnectionResult::CONNECTED:
      HnsVPNOSConnectionAPIBase::OnConnected();
      break;
    case CheckConnectionResult::CONNECTING:
      OnIsConnecting();
      break;
    case CheckConnectionResult::CONNECT_FAILED:
      OnConnectFailed();
      break;
    case CheckConnectionResult::DISCONNECTED:
      HnsVPNOSConnectionAPIBase::OnDisconnected();
      break;
    case CheckConnectionResult::DISCONNECTING:
      OnIsDisconnecting();
      break;
    default:
      break;
  }
}

void HnsVPNOSConnectionAPIWin::OnCreated(const std::string& name,
                                           const RasOperationResult& result) {
  if (!result.success) {
    SetLastConnectionError(result.error_description);
    OnCreateFailed();
    return;
  }

  HnsVPNOSConnectionAPIBase::OnCreated();
}

void HnsVPNOSConnectionAPIWin::OnConnected(const RasOperationResult& result) {
  if (!result.success) {
    SetLastConnectionError(result.error_description);
    HnsVPNOSConnectionAPIBase::OnConnectFailed();
  }
}

void HnsVPNOSConnectionAPIWin::OnDisconnected(
    const RasOperationResult& result) {
  // TODO(simonhong): Handle disconnect failed state.
  if (result.success) {
    HnsVPNOSConnectionAPIBase::OnDisconnected();
    return;
  }
  SetLastConnectionError(result.error_description);
}

bool HnsVPNOSConnectionAPIWin::IsPlatformNetworkAvailable() {
  // If any errors occur, return that internet connection is available.
  Microsoft::WRL::ComPtr<INetworkListManager> manager;
  HRESULT hr = ::CoCreateInstance(CLSID_NetworkListManager, nullptr, CLSCTX_ALL,
                                  IID_PPV_ARGS(&manager));
  if (FAILED(hr)) {
    LOG(ERROR) << "CoCreateInstance(NetworkListManager) hr=" << std::hex << hr;
    return true;
  }

  VARIANT_BOOL is_connected;
  hr = manager->get_IsConnectedToInternet(&is_connected);
  if (FAILED(hr)) {
    LOG(ERROR) << "get_IsConnectedToInternet failed hr=" << std::hex << hr;
    return true;
  }

  // Normally VARIANT_TRUE/VARIANT_FALSE are used with the type VARIANT_BOOL
  // but in this case the docs explicitly say to use FALSE.
  // https://docs.microsoft.com/en-us/windows/desktop/api/Netlistmgr/
  //     nf-netlistmgr-inetworklistmanager-get_isconnectedtointernet
  return is_connected != FALSE;
}

}  // namespace hns_vpn
