/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/browser/debounce/debounce_service_factory.h"

#include <memory>
#include <string>

#include "base/feature_list.h"
#include "base/no_destructor.h"
#include "hns/browser/hns_browser_process.h"
#include "hns/components/debounce/browser/debounce_service.h"
#include "hns/components/debounce/common/features.h"
#include "chrome/browser/profiles/incognito_helpers.h"
#include "chrome/browser/profiles/profile.h"
#include "components/keyed_service/content/browser_context_dependency_manager.h"
#include "components/keyed_service/core/keyed_service.h"

namespace debounce {

// static
DebounceServiceFactory* DebounceServiceFactory::GetInstance() {
  static base::NoDestructor<DebounceServiceFactory> instance;
  return instance.get();
}

DebounceService* DebounceServiceFactory::GetForBrowserContext(
    content::BrowserContext* context) {
  return static_cast<DebounceService*>(
      GetInstance()->GetServiceForBrowserContext(context, true));
}

DebounceServiceFactory::DebounceServiceFactory()
    : BrowserContextKeyedServiceFactory(
          "DebounceService",
          BrowserContextDependencyManager::GetInstance()) {}

DebounceServiceFactory::~DebounceServiceFactory() = default;

KeyedService* DebounceServiceFactory::BuildServiceInstanceFor(
    content::BrowserContext* context) const {
  // Don't create service is debounce feature is disabled
  if (!base::FeatureList::IsEnabled(debounce::features::kHnsDebounce))
    return nullptr;

  debounce::DebounceComponentInstaller* component_installer = nullptr;
  // Hns browser process may be null if we are being created within a unit
  // test.
  if (g_hns_browser_process)
    component_installer =
        g_hns_browser_process->debounce_component_installer();
  return new DebounceService(component_installer,
                             Profile::FromBrowserContext(context)->GetPrefs());
}

content::BrowserContext* DebounceServiceFactory::GetBrowserContextToUse(
    content::BrowserContext* context) const {
  return chrome::GetBrowserContextRedirectedInIncognito(context);
}

bool DebounceServiceFactory::ServiceIsNULLWhileTesting() const {
  return false;
}

}  // namespace debounce
