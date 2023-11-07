/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_UI_WEBUI_SETTINGS_HNS_VPN_HNS_VPN_HANDLER_H_
#define HNS_BROWSER_UI_WEBUI_SETTINGS_HNS_VPN_HNS_VPN_HANDLER_H_

#include <string>

#include "base/memory/raw_ptr.h"
#include "base/memory/weak_ptr.h"
#include "hns/components/hns_vpn/browser/hns_vpn_service_observer.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/ui/webui/settings/settings_page_ui_handler.h"
#include "components/prefs/pref_change_registrar.h"

class HnsVpnHandler : public settings::SettingsPageUIHandler,
                        public hns_vpn::HnsVPNServiceObserver {
 public:
  explicit HnsVpnHandler(Profile* profile);
  ~HnsVpnHandler() override;

 private:
  // WebUIMessageHandler implementation.
  void RegisterMessages() override;

  void HandleRegisterWireguardService(const base::Value::List& args);
  void HandleIsWireguardServiceRegistered(const base::Value::List& args);
  void OnWireguardServiceRegistered(const std::string& callback_id,
                                    bool success);
  void HandleIsHnsVpnConnected(const base::Value::List& args);

  // hns_vpn::HnsVPNServiceObserver
  void OnConnectionStateChanged(
      hns_vpn::mojom::ConnectionState state) override;
  void OnProtocolChanged();

  // SettingsPageUIHandler implementation.
  void OnJavascriptAllowed() override;
  void OnJavascriptDisallowed() override;

  PrefChangeRegistrar pref_change_registrar_;
  const raw_ptr<Profile, DanglingUntriaged> profile_;
  base::WeakPtrFactory<HnsVpnHandler> weak_factory_{this};
};

#endif  // HNS_BROWSER_UI_WEBUI_SETTINGS_HNS_VPN_HNS_VPN_HANDLER_H_
