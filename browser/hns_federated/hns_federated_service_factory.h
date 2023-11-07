/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_HNS_FEDERATED_HNS_FEDERATED_SERVICE_FACTORY_H_
#define HNS_BROWSER_HNS_FEDERATED_HNS_FEDERATED_SERVICE_FACTORY_H_

#include "hns/components/hns_federated/hns_federated_service.h"
#include "components/keyed_service/content/browser_context_keyed_service_factory.h"

namespace base {
template <typename T>
class NoDestructor;
}  // namespace base

namespace user_prefs {
class PrefRegistrySyncable;
}

namespace hns_federated {

// Singleton that owns all HnsFederatedService and associates them
// with Profiles.
class HnsFederatedServiceFactory : public BrowserContextKeyedServiceFactory {
 public:
  HnsFederatedServiceFactory(const HnsFederatedServiceFactory&) = delete;
  HnsFederatedServiceFactory& operator=(const HnsFederatedServiceFactory&) =
      delete;

  static hns_federated::HnsFederatedService* GetForBrowserContext(
      content::BrowserContext* context);
  static HnsFederatedServiceFactory* GetInstance();

 private:
  friend base::NoDestructor<HnsFederatedServiceFactory>;

  HnsFederatedServiceFactory();
  ~HnsFederatedServiceFactory() override;

  // BrowserContextKeyedServiceFactory:
  KeyedService* BuildServiceInstanceFor(
      content::BrowserContext* context) const override;
  bool ServiceIsCreatedWithBrowserContext() const override;
  void RegisterProfilePrefs(
      user_prefs::PrefRegistrySyncable* registry) override;
  bool ServiceIsNULLWhileTesting() const override;
};

}  // namespace hns_federated

#endif  // HNS_BROWSER_HNS_FEDERATED_HNS_FEDERATED_SERVICE_FACTORY_H_
