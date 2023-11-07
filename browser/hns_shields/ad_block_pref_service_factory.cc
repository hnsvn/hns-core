/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/browser/hns_shields/ad_block_pref_service_factory.h"

#include <utility>

#include "base/no_destructor.h"
#include "hns/browser/hns_browser_process.h"
#include "hns/components/hns_shields/browser/ad_block_pref_service.h"
#include "chrome/browser/net/proxy_service_factory.h"
#include "chrome/browser/profiles/incognito_helpers.h"
#include "chrome/browser/profiles/profile.h"
#include "components/keyed_service/content/browser_context_dependency_manager.h"
#include "components/proxy_config/pref_proxy_config_tracker.h"
#include "net/proxy_resolution/proxy_config_service.h"

namespace hns_shields {

// static
AdBlockPrefService* AdBlockPrefServiceFactory::GetForBrowserContext(
    content::BrowserContext* context) {
  return static_cast<AdBlockPrefService*>(
      GetInstance()->GetServiceForBrowserContext(context,
                                                 /*create_service=*/true));
}

// static
AdBlockPrefServiceFactory* AdBlockPrefServiceFactory::GetInstance() {
  static base::NoDestructor<AdBlockPrefServiceFactory> instance;
  return instance.get();
}

AdBlockPrefServiceFactory::AdBlockPrefServiceFactory()
    : BrowserContextKeyedServiceFactory(
          "AdBlockPrefService",
          BrowserContextDependencyManager::GetInstance()) {}

AdBlockPrefServiceFactory::~AdBlockPrefServiceFactory() = default;

KeyedService* AdBlockPrefServiceFactory::BuildServiceInstanceFor(
    content::BrowserContext* context) const {
  Profile* profile = Profile::FromBrowserContext(context);

  auto* service = new AdBlockPrefService(
      g_hns_browser_process->ad_block_service(), profile->GetPrefs());

  auto pref_proxy_config_tracker =
      ProxyServiceFactory::CreatePrefProxyConfigTrackerOfProfile(
          profile->GetPrefs(), nullptr);
  auto proxy_config_service = ProxyServiceFactory::CreateProxyConfigService(
      pref_proxy_config_tracker.get(), profile);
  service->StartProxyTracker(std::move(pref_proxy_config_tracker),
                             std::move(proxy_config_service));
  return service;
}

content::BrowserContext* AdBlockPrefServiceFactory::GetBrowserContextToUse(
    content::BrowserContext* context) const {
  return chrome::GetBrowserContextRedirectedInIncognito(context);
}

bool AdBlockPrefServiceFactory::ServiceIsCreatedWithBrowserContext() const {
  return true;
}

}  // namespace hns_shields
