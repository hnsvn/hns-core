/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/browser/sync/hns_sync_alerts_service_factory.h"

#include "base/no_destructor.h"
#include "hns/browser/sync/hns_sync_alerts_service.h"
#include "chrome/browser/profiles/incognito_helpers.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/sync/sync_service_factory.h"
#include "components/keyed_service/content/browser_context_dependency_manager.h"
#include "components/pref_registry/pref_registry_syncable.h"

// static
HnsSyncAlertsService* HnsSyncAlertsServiceFactory::GetForBrowserContext(
    content::BrowserContext* context) {
  return static_cast<HnsSyncAlertsService*>(
      GetInstance()->GetServiceForBrowserContext(context, true));
}

// static
HnsSyncAlertsServiceFactory* HnsSyncAlertsServiceFactory::GetInstance() {
  static base::NoDestructor<HnsSyncAlertsServiceFactory> instance;
  return instance.get();
}

HnsSyncAlertsServiceFactory::HnsSyncAlertsServiceFactory()
    : BrowserContextKeyedServiceFactory(
          "HnsSyncAlertsService",
          BrowserContextDependencyManager::GetInstance()) {}

HnsSyncAlertsServiceFactory::~HnsSyncAlertsServiceFactory() {}

KeyedService* HnsSyncAlertsServiceFactory::BuildServiceInstanceFor(
    content::BrowserContext* context) const {
  return new HnsSyncAlertsService(Profile::FromBrowserContext(context));
}

content::BrowserContext* HnsSyncAlertsServiceFactory::GetBrowserContextToUse(
    content::BrowserContext* context) const {
  return chrome::GetBrowserContextRedirectedInIncognito(context);
}

bool HnsSyncAlertsServiceFactory::ServiceIsCreatedWithBrowserContext() const {
  return true;
}
