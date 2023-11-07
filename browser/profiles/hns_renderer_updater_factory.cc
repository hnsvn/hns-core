/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/browser/profiles/hns_renderer_updater_factory.h"

#include "base/no_destructor.h"
#include "hns/browser/hns_wallet/keyring_service_factory.h"
#include "hns/browser/profiles/hns_renderer_updater.h"
#include "chrome/browser/profiles/incognito_helpers.h"
#include "chrome/browser/profiles/profile.h"
#include "components/keyed_service/content/browser_context_dependency_manager.h"

HnsRendererUpdaterFactory::HnsRendererUpdaterFactory()
    : BrowserContextKeyedServiceFactory(
          "HnsRendererUpdater",
          BrowserContextDependencyManager::GetInstance()) {
  DependsOn(hns_wallet::KeyringServiceFactory::GetInstance());
}

HnsRendererUpdaterFactory::~HnsRendererUpdaterFactory() = default;

// static
HnsRendererUpdaterFactory* HnsRendererUpdaterFactory::GetInstance() {
  static base::NoDestructor<HnsRendererUpdaterFactory> instance;
  return instance.get();
}

// static
HnsRendererUpdater* HnsRendererUpdaterFactory::GetForProfile(
    Profile* profile) {
  return static_cast<HnsRendererUpdater*>(
      GetInstance()->GetServiceForBrowserContext(profile, true));
}

KeyedService* HnsRendererUpdaterFactory::BuildServiceInstanceFor(
    content::BrowserContext* context) const {
  auto* keyring_service =
      hns_wallet::KeyringServiceFactory::GetServiceForContext(context);
  return new HnsRendererUpdater(static_cast<Profile*>(context),
                                  keyring_service);
}

bool HnsRendererUpdaterFactory::ServiceIsCreatedWithBrowserContext() const {
  return true;
}

content::BrowserContext* HnsRendererUpdaterFactory::GetBrowserContextToUse(
    content::BrowserContext* context) const {
  return chrome::GetBrowserContextRedirectedInIncognito(context);
}
