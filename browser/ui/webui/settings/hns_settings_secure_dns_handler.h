/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_UI_WEBUI_SETTINGS_HNS_SETTINGS_SECURE_DNS_HANDLER_H_
#define HNS_BROWSER_UI_WEBUI_SETTINGS_HNS_SETTINGS_SECURE_DNS_HANDLER_H_

#include "chrome/browser/ui/webui/settings/settings_secure_dns_handler.h"
#include "components/prefs/pref_change_registrar.h"

namespace settings {

class HnsSecureDnsHandler : public SecureDnsHandler {
 public:
  HnsSecureDnsHandler();
  ~HnsSecureDnsHandler() override;

  void OnJavascriptAllowed() override;
  void OnJavascriptDisallowed() override;

 private:
  PrefChangeRegistrar pref_registrar_;
};

}  // namespace settings

#endif  // HNS_BROWSER_UI_WEBUI_SETTINGS_HNS_SETTINGS_SECURE_DNS_HANDLER_H_
