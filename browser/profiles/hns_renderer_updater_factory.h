/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_PROFILES_HNS_RENDERER_UPDATER_FACTORY_H_
#define HNS_BROWSER_PROFILES_HNS_RENDERER_UPDATER_FACTORY_H_

#include "components/keyed_service/content/browser_context_keyed_service_factory.h"

class Profile;
class HnsRendererUpdater;

namespace base {
template <typename T>
class NoDestructor;
}  // namespace base

// Singleton that creates/deletes HnsRendererUpdater as new Profiles are
// created/shutdown.
class HnsRendererUpdaterFactory : public BrowserContextKeyedServiceFactory {
 public:
  // Returns an instance of the HnsRendererUpdaterFactory singleton.
  static HnsRendererUpdaterFactory* GetInstance();

  // Returns the instance of RendererUpdater for the passed |profile|.
  static HnsRendererUpdater* GetForProfile(Profile* profile);

  HnsRendererUpdaterFactory(const HnsRendererUpdaterFactory&) = delete;
  HnsRendererUpdaterFactory& operator=(const HnsRendererUpdaterFactory&) =
      delete;

 protected:
  // BrowserContextKeyedServiceFactory:
  KeyedService* BuildServiceInstanceFor(
      content::BrowserContext* profile) const override;
  bool ServiceIsCreatedWithBrowserContext() const override;

 private:
  friend base::NoDestructor<HnsRendererUpdaterFactory>;

  content::BrowserContext* GetBrowserContextToUse(
      content::BrowserContext* context) const override;

  HnsRendererUpdaterFactory();
  ~HnsRendererUpdaterFactory() override;
};

#endif  // HNS_BROWSER_PROFILES_HNS_RENDERER_UPDATER_FACTORY_H_
