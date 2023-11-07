/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_vpn/common/buildflags/buildflags.h"
#include "build/build_config.h"
#include "chrome/browser/browser_process.h"
#include "chrome/browser/net/secure_dns_config.h"
#include "chrome/browser/net/stub_resolver_config_reader.h"
#include "chrome/browser/net/system_network_context_manager.h"
#include "chrome/common/pref_names.h"
#include "chrome/grit/generated_resources.h"
#include "components/grit/hns_components_strings.h"
#include "components/prefs/pref_service.h"
#include "net/dns/public/secure_dns_mode.h"

#if BUILDFLAG(ENABLE_HNS_VPN)
#include "hns/components/hns_vpn/common/features.h"
#endif  // BUILDFLAG(ENABLE_HNS_VPN)

#if BUILDFLAG(ENABLE_HNS_VPN) && BUILDFLAG(IS_WIN)
namespace {

bool ShouldReplaceSecureDNSDisabledDescription() {
  if (!base::FeatureList::IsEnabled(
          hns_vpn::features::kHnsVPNDnsProtection))
    return false;
  auto dns_config = SystemNetworkContextManager::GetStubResolverConfigReader()
                        ->GetSecureDnsConfiguration(false);
  return !g_browser_process->local_state()
              ->GetString(prefs::kHnsVpnDnsConfig)
              .empty() ||
         dns_config.mode() == net::SecureDnsMode::kSecure;
}

}  // namespace

#define AddSecureDnsStrings AddSecureDnsStrings_ChromiumImpl

#endif  // BUILDFLAG(ENABLE_HNS_VPN) && BUILDFLAG(IS_WIN)

#include "src/chrome/browser/ui/webui/settings/shared_settings_localized_strings_provider.cc"

#if BUILDFLAG(ENABLE_HNS_VPN) && BUILDFLAG(IS_WIN)
#undef AddSecureDnsStrings
namespace settings {

void AddSecureDnsStrings(content::WebUIDataSource* html_source) {
  AddSecureDnsStrings_ChromiumImpl(html_source);
  if (!ShouldReplaceSecureDNSDisabledDescription())
    return;
  static constexpr webui::LocalizedString kLocalizedStrings[] = {
      {"secureDnsDisabledForManagedEnvironment",
       IDS_SETTINGS_SECURE_DNS_DISABLED_BY_HNS_VPN}};

  html_source->AddLocalizedStrings(kLocalizedStrings);
}

}  // namespace settings

#endif  // BUILDFLAG(ENABLE_HNS_VPN) && BUILDFLAG(IS_WIN)
