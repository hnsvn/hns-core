/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/browser/ui/webui/settings/hns_settings_secure_dns_handler.h"

#include "base/feature_list.h"
#include "hns/components/hns_vpn/common/features.h"
#include "chrome/browser/browser_process.h"
#include "chrome/common/pref_names.h"
#include "components/prefs/pref_service.h"

namespace settings {

HnsSecureDnsHandler::HnsSecureDnsHandler() = default;
HnsSecureDnsHandler::~HnsSecureDnsHandler() = default;

void HnsSecureDnsHandler::OnJavascriptAllowed() {
  SecureDnsHandler::OnJavascriptAllowed();
  pref_registrar_.Init(g_browser_process->local_state());
  if (base::FeatureList::IsEnabled(
          hns_vpn::features::kHnsVPNDnsProtection)) {
    pref_registrar_.Add(
        prefs::kHnsVpnDnsConfig,
        base::BindRepeating(
            &HnsSecureDnsHandler::SendSecureDnsSettingUpdatesToJavascript,
            base::Unretained(this)));
  }
}

void HnsSecureDnsHandler::OnJavascriptDisallowed() {
  SecureDnsHandler::OnJavascriptDisallowed();
  pref_registrar_.RemoveAll();
}

}  // namespace settings
