/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_UI_SIDEBAR_SIDEBAR_SERVICE_FACTORY_H_
#define HNS_BROWSER_UI_SIDEBAR_SIDEBAR_SERVICE_FACTORY_H_

#include "components/keyed_service/content/browser_context_keyed_service_factory.h"
#include "components/keyed_service/core/keyed_service.h"

class Profile;

namespace base {
template <typename T>
class NoDestructor;
}  // namespace base

namespace sidebar {

class SidebarService;

class SidebarServiceFactory : public BrowserContextKeyedServiceFactory {
 public:
  static SidebarServiceFactory* GetInstance();
  static SidebarService* GetForProfile(Profile* profile);

 private:
  friend base::NoDestructor<SidebarServiceFactory>;

  SidebarServiceFactory();
  ~SidebarServiceFactory() override;

  SidebarServiceFactory(const SidebarServiceFactory&) = delete;
  SidebarServiceFactory& operator=(const SidebarServiceFactory&) = delete;

  // BrowserContextKeyedServiceFactory overrides:
  KeyedService* BuildServiceInstanceFor(
      content::BrowserContext* context) const override;
  content::BrowserContext* GetBrowserContextToUse(
      content::BrowserContext* context) const override;
};

}  // namespace sidebar

#endif  // HNS_BROWSER_UI_SIDEBAR_SIDEBAR_SERVICE_FACTORY_H_
