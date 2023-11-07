/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/browser/hns_vpn/hns_vpn_service_factory.h"

#include <utility>

#include "base/feature_list.h"
#include "base/no_destructor.h"
#include "hns/browser/hns_browser_process.h"
#include "hns/browser/hns_vpn/vpn_utils.h"
#include "hns/browser/profiles/profile_util.h"
#include "hns/browser/skus/skus_service_factory.h"
#include "hns/components/hns_vpn/browser/hns_vpn_service.h"
#include "hns/components/hns_vpn/common/hns_vpn_utils.h"
#include "hns/components/skus/common/features.h"
#include "build/build_config.h"
#include "chrome/browser/browser_process.h"
#include "chrome/browser/profiles/incognito_helpers.h"
#include "chrome/browser/profiles/profile.h"
#include "components/keyed_service/content/browser_context_dependency_manager.h"
#include "components/user_prefs/user_prefs.h"
#include "content/public/browser/browser_context.h"
#include "content/public/browser/storage_partition.h"

#if BUILDFLAG(IS_WIN)
#include "hns/browser/hns_vpn/dns/hns_vpn_dns_observer_factory_win.h"
#include "hns/browser/hns_vpn/dns/hns_vpn_dns_observer_service_win.h"
#include "hns/browser/hns_vpn/win/hns_vpn_wireguard_observer_factory_win.h"
#include "hns/browser/hns_vpn/win/hns_vpn_wireguard_observer_service_win.h"
#endif

namespace hns_vpn {

// static
HnsVpnServiceFactory* HnsVpnServiceFactory::GetInstance() {
  static base::NoDestructor<HnsVpnServiceFactory> instance;
  return instance.get();
}

// static
HnsVpnService* HnsVpnServiceFactory::GetForProfile(Profile* profile) {
  return static_cast<HnsVpnService*>(
      GetInstance()->GetServiceForBrowserContext(profile, true));
}

// static
void HnsVpnServiceFactory::BindForContext(
    content::BrowserContext* context,
    mojo::PendingReceiver<hns_vpn::mojom::ServiceHandler> receiver) {
  auto* service = static_cast<HnsVpnService*>(
      GetInstance()->GetServiceForBrowserContext(context, true));
  if (service) {
    service->BindInterface(std::move(receiver));
  }
}

HnsVpnServiceFactory::HnsVpnServiceFactory()
    : BrowserContextKeyedServiceFactory(
          "HnsVpnService",
          BrowserContextDependencyManager::GetInstance()) {
  DependsOn(skus::SkusServiceFactory::GetInstance());
#if BUILDFLAG(IS_WIN)
  if (hns_vpn::IsHnsVPNWireguardEnabled(g_browser_process->local_state())) {
    DependsOn(hns_vpn::HnsVpnWireguardObserverFactory::GetInstance());
  } else {
    DependsOn(hns_vpn::HnsVpnDnsObserverFactory::GetInstance());
  }
#endif
}

HnsVpnServiceFactory::~HnsVpnServiceFactory() = default;

KeyedService* HnsVpnServiceFactory::BuildServiceInstanceFor(
    content::BrowserContext* context) const {
  if (!hns_vpn::IsAllowedForContext(context)) {
    return nullptr;
  }

#if !BUILDFLAG(IS_ANDROID)
  if (!g_hns_browser_process->hns_vpn_os_connection_api()) {
    return nullptr;
  }
#endif

  auto* default_storage_partition = context->GetDefaultStoragePartition();
  auto shared_url_loader_factory =
      default_storage_partition->GetURLLoaderFactoryForBrowserProcess();
  auto* local_state = g_browser_process->local_state();
  hns_vpn::MigrateVPNSettings(user_prefs::UserPrefs::Get(context),
                                local_state);
  auto callback = base::BindRepeating(
      [](content::BrowserContext* context) {
        return skus::SkusServiceFactory::GetForContext(context);
      },
      context);

  auto* vpn_service = new HnsVpnService(
      g_hns_browser_process->hns_vpn_os_connection_api(),
      shared_url_loader_factory, local_state,
      user_prefs::UserPrefs::Get(context), callback);
#if BUILDFLAG(IS_WIN)
  if (hns_vpn::IsHnsVPNWireguardEnabled(g_browser_process->local_state())) {
    auto* observer_service =
        hns_vpn::HnsVpnWireguardObserverFactory::GetInstance()
            ->GetServiceForContext(context);
    if (observer_service) {
      observer_service->Observe(vpn_service);
    }
  } else {
    auto* observer_service =
        hns_vpn::HnsVpnDnsObserverFactory::GetInstance()
            ->GetServiceForContext(context);
    if (observer_service) {
      observer_service->Observe(vpn_service);
    }
  }
#endif
  return vpn_service;
}

void HnsVpnServiceFactory::RegisterProfilePrefs(
    user_prefs::PrefRegistrySyncable* registry) {
  hns_vpn::RegisterProfilePrefs(registry);
}

}  // namespace hns_vpn
