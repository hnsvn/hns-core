/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_SYNC_HNS_SYNC_ALERTS_SERVICE_FACTORY_H_
#define HNS_BROWSER_SYNC_HNS_SYNC_ALERTS_SERVICE_FACTORY_H_

#include "components/keyed_service/content/browser_context_keyed_service_factory.h"

namespace base {
template <typename T>
class NoDestructor;
}  // namespace base

class HnsSyncAlertsService;

class HnsSyncAlertsServiceFactory : public BrowserContextKeyedServiceFactory {
 public:
  HnsSyncAlertsServiceFactory(const HnsSyncAlertsServiceFactory&) = delete;
  HnsSyncAlertsServiceFactory& operator=(
      const HnsSyncAlertsServiceFactory&) = delete;

  static HnsSyncAlertsService* GetForBrowserContext(
      content::BrowserContext* context);

  static HnsSyncAlertsServiceFactory* GetInstance();

 private:
  friend base::NoDestructor<HnsSyncAlertsServiceFactory>;

  HnsSyncAlertsServiceFactory();
  ~HnsSyncAlertsServiceFactory() override;

  // BrowserContextKeyedServiceFactory:
  KeyedService* BuildServiceInstanceFor(
      content::BrowserContext* profile) const override;
  content::BrowserContext* GetBrowserContextToUse(
      content::BrowserContext* context) const override;
  bool ServiceIsCreatedWithBrowserContext() const override;
};

#endif  // HNS_BROWSER_SYNC_HNS_SYNC_ALERTS_SERVICE_FACTORY_H_
