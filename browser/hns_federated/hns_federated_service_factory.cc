/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/browser/hns_federated/hns_federated_service_factory.h"

#include <memory>
#include <string>
#include <utility>

#include "base/no_destructor.h"
#include "hns/components/hns_federated/hns_federated_service.h"
#include "hns/components/hns_federated/features.h"
#include "chrome/browser/browser_process.h"
#include "components/keyed_service/content/browser_context_dependency_manager.h"
#include "components/pref_registry/pref_registry_syncable.h"
#include "components/user_prefs/user_prefs.h"
#include "content/public/browser/browser_context.h"
#include "content/public/browser/storage_partition.h"
#include "services/network/public/cpp/shared_url_loader_factory.h"

namespace hns_federated {

// static
HnsFederatedService* HnsFederatedServiceFactory::GetForBrowserContext(
    content::BrowserContext* context) {
  return static_cast<HnsFederatedService*>(
      GetInstance()->GetServiceForBrowserContext(context, true));
}

// static
HnsFederatedServiceFactory* HnsFederatedServiceFactory::GetInstance() {
  static base::NoDestructor<HnsFederatedServiceFactory> instance;
  return instance.get();
}

HnsFederatedServiceFactory::HnsFederatedServiceFactory()
    : BrowserContextKeyedServiceFactory(
          "HnsFederatedService",
          BrowserContextDependencyManager::GetInstance()) {}

HnsFederatedServiceFactory::~HnsFederatedServiceFactory() = default;

KeyedService* HnsFederatedServiceFactory::BuildServiceInstanceFor(
    content::BrowserContext* context) const {
  if (!base::FeatureList::IsEnabled(features::kFederatedLearning)) {
    return nullptr;
  }

  auto url_loader_factory = context->GetDefaultStoragePartition()
                                ->GetURLLoaderFactoryForBrowserProcess();
  std::unique_ptr<HnsFederatedService> hns_federated_service(
      new HnsFederatedService(user_prefs::UserPrefs::Get(context),
                                g_browser_process->local_state(),
                                context->GetPath(), url_loader_factory));
  return hns_federated_service.release();
}

void HnsFederatedServiceFactory::RegisterProfilePrefs(
    user_prefs::PrefRegistrySyncable* registry) {
  HnsFederatedService::RegisterProfilePrefs(registry);
}

bool HnsFederatedServiceFactory::ServiceIsCreatedWithBrowserContext() const {
  return true;
}

bool HnsFederatedServiceFactory::ServiceIsNULLWhileTesting() const {
  return false;
}

}  // namespace hns_federated
