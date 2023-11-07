/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/ios/browser/hns_wallet/swap_service_factory.h"

#include "base/no_destructor.h"
#include "hns/components/hns_wallet/browser/swap_service.h"
#include "hns/ios/browser/hns_wallet/json_rpc_service_factory.h"
#include "components/keyed_service/core/keyed_service.h"
#include "components/keyed_service/ios/browser_state_dependency_manager.h"
#include "ios/chrome/browser/shared/model/browser_state/browser_state_otr_helper.h"
#include "ios/chrome/browser/shared/model/browser_state/chrome_browser_state.h"
#include "services/network/public/cpp/shared_url_loader_factory.h"

namespace hns_wallet {

// static
mojo::PendingRemote<mojom::SwapService> SwapServiceFactory::GetForBrowserState(
    ChromeBrowserState* browser_state) {
  return static_cast<SwapService*>(
             GetInstance()->GetServiceForBrowserState(browser_state, true))
      ->MakeRemote();
}

// static
SwapServiceFactory* SwapServiceFactory::GetInstance() {
  static base::NoDestructor<SwapServiceFactory> instance;
  return instance.get();
}

SwapServiceFactory::SwapServiceFactory()
    : BrowserStateKeyedServiceFactory(
          "SwapService",
          BrowserStateDependencyManager::GetInstance()) {
  DependsOn(JsonRpcServiceFactory::GetInstance());
}

SwapServiceFactory::~SwapServiceFactory() = default;

std::unique_ptr<KeyedService> SwapServiceFactory::BuildServiceInstanceFor(
    web::BrowserState* context) const {
  auto* browser_state = ChromeBrowserState::FromBrowserState(context);
  auto* json_rpc_service =
      JsonRpcServiceFactory::GetServiceForState(browser_state);
  std::unique_ptr<SwapService> swap_service(new SwapService(
      browser_state->GetSharedURLLoaderFactory(), json_rpc_service));
  return swap_service;
}

bool SwapServiceFactory::ServiceIsNULLWhileTesting() const {
  return true;
}

web::BrowserState* SwapServiceFactory::GetBrowserStateToUse(
    web::BrowserState* context) const {
  return GetBrowserStateRedirectedInIncognito(context);
}

}  // namespace hns_wallet
