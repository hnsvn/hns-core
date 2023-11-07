// Copyright (c) 2021 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

#include "hns/browser/hns_news/hns_news_controller_factory.h"

#include "base/no_destructor.h"
#include "hns/browser/hns_ads/ads_service_factory.h"
#include "hns/browser/profiles/profile_util.h"
#include "hns/components/hns_news/browser/hns_news_controller.h"
#include "chrome/browser/favicon/favicon_service_factory.h"
#include "chrome/browser/history/history_service_factory.h"
#include "chrome/browser/profiles/incognito_helpers.h"
#include "chrome/browser/profiles/profile.h"
#include "components/favicon/core/favicon_service.h"
#include "components/keyed_service/content/browser_context_dependency_manager.h"
#include "components/keyed_service/core/service_access_type.h"
#include "content/public/browser/browser_context.h"
#include "services/network/public/cpp/shared_url_loader_factory.h"

namespace hns_news {

// static
HnsNewsControllerFactory* HnsNewsControllerFactory::GetInstance() {
  static base::NoDestructor<HnsNewsControllerFactory> instance;
  return instance.get();
}

// static
HnsNewsController* HnsNewsControllerFactory::GetForContext(
    content::BrowserContext* context) {
  return static_cast<HnsNewsController*>(
      GetInstance()->GetServiceForBrowserContext(context, true));
}

// static
mojo::PendingRemote<mojom::HnsNewsController>
HnsNewsControllerFactory::GetRemoteService(content::BrowserContext* context) {
  return static_cast<HnsNewsController*>(
             GetInstance()->GetServiceForBrowserContext(context, true))
      ->MakeRemote();
}

// static
HnsNewsController* HnsNewsControllerFactory::GetControllerForContext(
    content::BrowserContext* context) {
  return static_cast<HnsNewsController*>(
      GetInstance()->GetServiceForBrowserContext(context, true));
}

HnsNewsControllerFactory::HnsNewsControllerFactory()
    : BrowserContextKeyedServiceFactory(
          "HnsNewsControllerFactory",
          BrowserContextDependencyManager::GetInstance()) {
  DependsOn(hns_ads::AdsServiceFactory::GetInstance());
  DependsOn(FaviconServiceFactory::GetInstance());
  DependsOn(HistoryServiceFactory::GetInstance());
}

HnsNewsControllerFactory::~HnsNewsControllerFactory() = default;

KeyedService* HnsNewsControllerFactory::BuildServiceInstanceFor(
    content::BrowserContext* context) const {
  if (!hns::IsRegularProfile(context)) {
    return nullptr;
  }
  auto* profile = Profile::FromBrowserContext(context);
  if (!profile) {
    return nullptr;
  }
  auto* favicon_service = FaviconServiceFactory::GetForProfile(
      profile, ServiceAccessType::EXPLICIT_ACCESS);
  auto* ads_service = hns_ads::AdsServiceFactory::GetForProfile(profile);
  auto* history_service = HistoryServiceFactory::GetForProfile(
      profile, ServiceAccessType::EXPLICIT_ACCESS);
  return new HnsNewsController(profile->GetPrefs(), favicon_service,
                                 ads_service, history_service,
                                 profile->GetURLLoaderFactory());
}

}  // namespace hns_news
