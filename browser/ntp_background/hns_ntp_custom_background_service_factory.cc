// Copyright (c) 2021 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#include "hns/browser/ntp_background/hns_ntp_custom_background_service_factory.h"

#include <memory>

#include "base/no_destructor.h"
#include "hns/browser/ntp_background/hns_ntp_custom_background_service_delegate.h"
#include "hns/browser/profiles/profile_util.h"
#include "hns/components/ntp_background_images/browser/hns_ntp_custom_background_service.h"
#include "chrome/browser/profiles/profile.h"
#include "components/keyed_service/content/browser_context_dependency_manager.h"

// static
ntp_background_images::HnsNTPCustomBackgroundService*
HnsNTPCustomBackgroundServiceFactory::GetForContext(
    content::BrowserContext* context) {
  return static_cast<ntp_background_images::HnsNTPCustomBackgroundService*>(
      GetInstance()->GetServiceForBrowserContext(context, true));
}

// static
HnsNTPCustomBackgroundServiceFactory*
HnsNTPCustomBackgroundServiceFactory::GetInstance() {
  static base::NoDestructor<HnsNTPCustomBackgroundServiceFactory> instance;
  return instance.get();
}

HnsNTPCustomBackgroundServiceFactory::HnsNTPCustomBackgroundServiceFactory()
    : BrowserContextKeyedServiceFactory(
          "HnsNTPCustomBackgroundService",
          BrowserContextDependencyManager::GetInstance()) {}

HnsNTPCustomBackgroundServiceFactory::
    ~HnsNTPCustomBackgroundServiceFactory() = default;

KeyedService* HnsNTPCustomBackgroundServiceFactory::BuildServiceInstanceFor(
    content::BrowserContext* context) const {
  // Custom NTP background is only used in normal profile.
  if (!hns::IsRegularProfile(context)) {
    return nullptr;
  }

  return new ntp_background_images::HnsNTPCustomBackgroundService(
      std::make_unique<HnsNTPCustomBackgroundServiceDelegate>(
          Profile::FromBrowserContext(context)));
}
