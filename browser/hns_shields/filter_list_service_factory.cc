/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/browser/hns_shields/filter_list_service_factory.h"

#include <utility>

#include "base/no_destructor.h"
#include "hns/browser/hns_browser_process.h"
#include "hns/components/hns_shields/browser/filter_list_service.h"
#include "chrome/browser/browser_process.h"
#include "chrome/browser/profiles/incognito_helpers.h"
#include "components/keyed_service/content/browser_context_dependency_manager.h"

namespace hns_shields {

// static
FilterListServiceFactory* FilterListServiceFactory::GetInstance() {
  static base::NoDestructor<FilterListServiceFactory> instance;
  return instance.get();
}

// static
mojo::PendingRemote<mojom::FilterListAndroidHandler>
FilterListServiceFactory::GetForContext(content::BrowserContext* context) {
  return static_cast<FilterListService*>(
             GetInstance()->GetServiceForBrowserContext(context, true))
      ->MakeRemote();
}

// static
FilterListService* FilterListServiceFactory::GetServiceForContext(
    content::BrowserContext* context) {
  return static_cast<FilterListService*>(
      GetInstance()->GetServiceForBrowserContext(context, true));
}

// static
void FilterListServiceFactory::BindForContext(
    content::BrowserContext* context,
    mojo::PendingReceiver<mojom::FilterListAndroidHandler> receiver) {
  auto* filter_list_opt_in_service =
      FilterListServiceFactory::GetServiceForContext(context);
  if (filter_list_opt_in_service) {
    filter_list_opt_in_service->Bind(std::move(receiver));
  }
}

FilterListServiceFactory::FilterListServiceFactory()
    : BrowserContextKeyedServiceFactory(
          "FilterListService",
          BrowserContextDependencyManager::GetInstance()) {}

FilterListServiceFactory::~FilterListServiceFactory() = default;

KeyedService* FilterListServiceFactory::BuildServiceInstanceFor(
    content::BrowserContext* context) const {
  auto* ad_block_service = g_hns_browser_process->ad_block_service();
  return new FilterListService(ad_block_service);
}

content::BrowserContext* FilterListServiceFactory::GetBrowserContextToUse(
    content::BrowserContext* context) const {
  return chrome::GetBrowserContextRedirectedInIncognito(context);
}

}  // namespace hns_shields
