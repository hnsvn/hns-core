/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/browser/hns_vpn/dns/hns_vpn_dns_observer_factory_win.h"

#include <cstddef>
#include <memory>
#include <string>

#include "base/feature_list.h"
#include "base/no_destructor.h"
#include "hns/browser/hns_vpn/dns/hns_vpn_dns_observer_service_win.h"
#include "hns/browser/hns_vpn/vpn_utils.h"
#include "hns/components/hns_vpn/common/hns_vpn_utils.h"
#include "hns/components/hns_vpn/common/features.h"
#include "hns/components/hns_vpn/common/pref_names.h"
#include "chrome/browser/browser_process.h"
#include "components/keyed_service/content/browser_context_dependency_manager.h"
#include "components/pref_registry/pref_registry_syncable.h"
#include "components/user_prefs/user_prefs.h"
#include "content/public/browser/browser_context.h"

namespace hns_vpn {

// static
HnsVpnDnsObserverFactory* HnsVpnDnsObserverFactory::GetInstance() {
  static base::NoDestructor<HnsVpnDnsObserverFactory> instance;
  return instance.get();
}

HnsVpnDnsObserverFactory::~HnsVpnDnsObserverFactory() = default;

HnsVpnDnsObserverFactory::HnsVpnDnsObserverFactory()
    : BrowserContextKeyedServiceFactory(
          "HnsVpnDNSObserverService",
          BrowserContextDependencyManager::GetInstance()) {}

KeyedService* HnsVpnDnsObserverFactory::BuildServiceInstanceFor(
    content::BrowserContext* context) const {
  return new HnsVpnDnsObserverService(g_browser_process->local_state(),
                                        user_prefs::UserPrefs::Get(context));
}

// static
HnsVpnDnsObserverService* HnsVpnDnsObserverFactory::GetServiceForContext(
    content::BrowserContext* context) {
  if (!base::FeatureList::IsEnabled(
          hns_vpn::features::kHnsVPNDnsProtection) ||
      hns_vpn::IsHnsVPNWireguardEnabled(g_browser_process->local_state())) {
    return nullptr;
  }
  DCHECK(hns_vpn::IsAllowedForContext(context));
  return static_cast<HnsVpnDnsObserverService*>(
      GetInstance()->GetServiceForBrowserContext(context, true));
}

void HnsVpnDnsObserverFactory::RegisterProfilePrefs(
    user_prefs::PrefRegistrySyncable* registry) {
  registry->RegisterBooleanPref(prefs::kHnsVpnShowDNSPolicyWarningDialog,
                                true);
}

}  // namespace hns_vpn
