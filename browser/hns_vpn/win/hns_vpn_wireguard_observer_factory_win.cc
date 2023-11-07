/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/browser/hns_vpn/win/hns_vpn_wireguard_observer_factory_win.h"

#include "base/feature_list.h"
#include "base/no_destructor.h"
#include "hns/browser/hns_vpn/vpn_utils.h"
#include "hns/browser/hns_vpn/win/hns_vpn_wireguard_observer_service_win.h"
#include "hns/components/hns_vpn/common/hns_vpn_utils.h"
#include "chrome/browser/browser_process.h"
#include "components/keyed_service/content/browser_context_dependency_manager.h"
#include "content/public/browser/browser_context.h"

namespace hns_vpn {

// static
HnsVpnWireguardObserverFactory*
HnsVpnWireguardObserverFactory::GetInstance() {
  static base::NoDestructor<HnsVpnWireguardObserverFactory> instance;
  return instance.get();
}

HnsVpnWireguardObserverFactory::~HnsVpnWireguardObserverFactory() = default;

HnsVpnWireguardObserverFactory::HnsVpnWireguardObserverFactory()
    : BrowserContextKeyedServiceFactory(
          "HnsVpnWireguardObserverService",
          BrowserContextDependencyManager::GetInstance()) {}

KeyedService* HnsVpnWireguardObserverFactory::BuildServiceInstanceFor(
    content::BrowserContext* context) const {
  return new HnsVpnWireguardObserverService();
}

// static
HnsVpnWireguardObserverService*
HnsVpnWireguardObserverFactory::GetServiceForContext(
    content::BrowserContext* context) {
  DCHECK(
      hns_vpn::IsHnsVPNWireguardEnabled(g_browser_process->local_state()));
  DCHECK(hns_vpn::IsAllowedForContext(context));
  return static_cast<HnsVpnWireguardObserverService*>(
      GetInstance()->GetServiceForBrowserContext(context, true));
}

}  // namespace hns_vpn
