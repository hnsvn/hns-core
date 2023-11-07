/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/browser/ui/webui/settings/hns_vpn/hns_vpn_handler.h"

#include <memory>

#include "base/command_line.h"
#include "base/files/file_path.h"
#include "base/path_service.h"
#include "base/process/launch.h"
#include "base/task/thread_pool.h"
#include "hns/browser/hns_vpn/hns_vpn_service_factory.h"
#include "hns/components/hns_vpn/browser/hns_vpn_service.h"
#include "hns/components/hns_vpn/common/hns_vpn_utils.h"
#include "hns/components/hns_vpn/common/pref_names.h"
#include "hns/components/hns_vpn/common/wireguard/win/service_constants.h"
#include "hns/components/hns_vpn/common/wireguard/win/service_details.h"
#include "hns/components/hns_vpn/common/wireguard/win/storage_utils.h"
#include "hns/components/hns_vpn/common/wireguard/win/wireguard_utils_win.h"
#include "chrome/browser/browser_process.h"
#include "components/prefs/pref_service.h"
#include "components/version_info/version_info.h"

namespace {

bool ElevatedRegisterHnsVPNService() {
  auto executable_path = hns_vpn::GetHnsVPNWireguardServiceExecutablePath();
  base::CommandLine cmd(executable_path);
  cmd.AppendSwitch(hns_vpn::kHnsVpnWireguardServiceInstallSwitchName);
  base::LaunchOptions options = base::LaunchOptions();
  options.wait = true;
  options.elevated = true;
  return base::LaunchProcess(cmd, options).IsValid();
}

}  // namespace

HnsVpnHandler::HnsVpnHandler(Profile* profile) : profile_(profile) {
  auto* service = hns_vpn::HnsVpnServiceFactory::GetForProfile(profile);
  CHECK(service);
  Observe(service);

  pref_change_registrar_.Init(g_browser_process->local_state());
  pref_change_registrar_.Add(
      hns_vpn::prefs::kHnsVPNWireguardEnabled,
      base::BindRepeating(&HnsVpnHandler::OnProtocolChanged,
                          base::Unretained(this)));
}

HnsVpnHandler::~HnsVpnHandler() = default;

void HnsVpnHandler::RegisterMessages() {
  web_ui()->RegisterMessageCallback(
      "registerWireguardService",
      base::BindRepeating(&HnsVpnHandler::HandleRegisterWireguardService,
                          base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
      "isWireguardServiceRegistered",
      base::BindRepeating(&HnsVpnHandler::HandleIsWireguardServiceRegistered,
                          base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
      "isHnsVpnConnected",
      base::BindRepeating(&HnsVpnHandler::HandleIsHnsVpnConnected,
                          base::Unretained(this)));
}

void HnsVpnHandler::OnProtocolChanged() {
  auto enabled =
      hns_vpn::IsHnsVPNWireguardEnabled(g_browser_process->local_state());
  hns_vpn::SetWireguardActive(enabled);
}

void HnsVpnHandler::HandleRegisterWireguardService(
    const base::Value::List& args) {
  base::ThreadPool::PostTaskAndReplyWithResult(
      FROM_HERE,
      {base::MayBlock(), base::TaskPriority::BEST_EFFORT,
       base::TaskShutdownBehavior::CONTINUE_ON_SHUTDOWN},
      base::BindOnce(&ElevatedRegisterHnsVPNService),
      base::BindOnce(&HnsVpnHandler::OnWireguardServiceRegistered,
                     weak_factory_.GetWeakPtr(), args[0].GetString()));
}

void HnsVpnHandler::OnWireguardServiceRegistered(
    const std::string& callback_id,
    bool success) {
  AllowJavascript();
  ResolveJavascriptCallback(callback_id, base::Value(success));
}

void HnsVpnHandler::HandleIsWireguardServiceRegistered(
    const base::Value::List& args) {
  AllowJavascript();

  ResolveJavascriptCallback(
      args[0],
      base::Value(hns_vpn::wireguard::IsWireguardServiceRegistered()));
}

void HnsVpnHandler::HandleIsHnsVpnConnected(const base::Value::List& args) {
  AllowJavascript();

  auto* service = hns_vpn::HnsVpnServiceFactory::GetForProfile(profile_);
  ResolveJavascriptCallback(args[0],
                            base::Value(service && service->IsConnected()));
}

void HnsVpnHandler::OnConnectionStateChanged(
    hns_vpn::mojom::ConnectionState state) {
  AllowJavascript();
  FireWebUIListener(
      "hns-vpn-state-change",
      base::Value(state == hns_vpn::mojom::ConnectionState::CONNECTED));
}

void HnsVpnHandler::OnJavascriptAllowed() {}

void HnsVpnHandler::OnJavascriptDisallowed() {}
