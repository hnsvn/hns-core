/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_HNS_SHIELDS_AD_BLOCK_PREF_SERVICE_FACTORY_H_
#define HNS_BROWSER_HNS_SHIELDS_AD_BLOCK_PREF_SERVICE_FACTORY_H_

#include "components/keyed_service/content/browser_context_keyed_service_factory.h"

namespace base {
template <typename T>
class NoDestructor;
}  // namespace base

namespace hns_shields {

class AdBlockPrefService;

class AdBlockPrefServiceFactory : public BrowserContextKeyedServiceFactory {
 public:
  AdBlockPrefServiceFactory(const AdBlockPrefServiceFactory&) = delete;
  AdBlockPrefServiceFactory& operator=(const AdBlockPrefServiceFactory&) =
      delete;

  static AdBlockPrefService* GetForBrowserContext(
      content::BrowserContext* context);

  static AdBlockPrefServiceFactory* GetInstance();

 private:
  friend base::NoDestructor<AdBlockPrefServiceFactory>;

  AdBlockPrefServiceFactory();
  ~AdBlockPrefServiceFactory() override;

  // BrowserContextKeyedServiceFactory:
  KeyedService* BuildServiceInstanceFor(
      content::BrowserContext* profile) const override;

  // We use the same service in both normal and incognito modes.
  content::BrowserContext* GetBrowserContextToUse(
      content::BrowserContext* context) const override;

  bool ServiceIsCreatedWithBrowserContext() const override;
};

}  // namespace hns_shields

#endif  // HNS_BROWSER_HNS_SHIELDS_AD_BLOCK_PREF_SERVICE_FACTORY_H_
