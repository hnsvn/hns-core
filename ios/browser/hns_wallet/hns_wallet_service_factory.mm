/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/ios/browser/hns_wallet/hns_wallet_service_factory.h"

#include <memory>

#include "hns/components/hns_wallet/browser/hns_wallet_service.h"
#include "hns/components/hns_wallet/browser/hns_wallet_service_delegate.h"
#include "hns/ios/browser/hns_wallet/json_rpc_service_factory.h"
#include "hns/ios/browser/hns_wallet/keyring_service_factory.h"
#include "hns/ios/browser/hns_wallet/tx_service_factory.h"
#include "components/keyed_service/core/keyed_service.h"
#include "components/keyed_service/ios/browser_state_dependency_manager.h"
#include "ios/chrome/browser/shared/model/application_context/application_context.h"
#include "ios/chrome/browser/shared/model/browser_state/browser_state_otr_helper.h"
#include "ios/chrome/browser/shared/model/browser_state/chrome_browser_state.h"
#include "ios/web/public/browser_state.h"
#include "services/network/public/cpp/shared_url_loader_factory.h"

namespace hns_wallet {

// static
mojo::PendingRemote<mojom::HnsWalletService>
HnsWalletServiceFactory::GetForBrowserState(
    ChromeBrowserState* browser_state) {
  return static_cast<HnsWalletService*>(
             GetInstance()->GetServiceForBrowserState(browser_state, true))
      ->MakeRemote();
}

// static
HnsWalletService* HnsWalletServiceFactory::GetServiceForState(
    ChromeBrowserState* browser_state) {
  return static_cast<HnsWalletService*>(
      GetInstance()->GetServiceForBrowserState(browser_state, true));
}

// static
HnsWalletServiceFactory* HnsWalletServiceFactory::GetInstance() {
  static base::NoDestructor<HnsWalletServiceFactory> instance;
  return instance.get();
}

HnsWalletServiceFactory::HnsWalletServiceFactory()
    : BrowserStateKeyedServiceFactory(
          "HnsWalletService",
          BrowserStateDependencyManager::GetInstance()) {
  DependsOn(KeyringServiceFactory::GetInstance());
  DependsOn(JsonRpcServiceFactory::GetInstance());
  DependsOn(TxServiceFactory::GetInstance());
}

HnsWalletServiceFactory::~HnsWalletServiceFactory() = default;

std::unique_ptr<KeyedService>
HnsWalletServiceFactory::BuildServiceInstanceFor(
    web::BrowserState* context) const {
  auto* browser_state = ChromeBrowserState::FromBrowserState(context);
  auto* keyring_service =
      KeyringServiceFactory::GetServiceForState(browser_state);
  auto* json_rpc_service =
      JsonRpcServiceFactory::GetServiceForState(browser_state);
  auto* tx_service = TxServiceFactory::GetServiceForState(browser_state);
  auto shared_url_loader_factory = browser_state->GetSharedURLLoaderFactory();
  std::unique_ptr<HnsWalletService> service(new HnsWalletService(
      shared_url_loader_factory, std::make_unique<HnsWalletServiceDelegate>(),
      keyring_service, json_rpc_service, tx_service, browser_state->GetPrefs(),
      GetApplicationContext()->GetLocalState()));
  return service;
}

bool HnsWalletServiceFactory::ServiceIsNULLWhileTesting() const {
  return true;
}

web::BrowserState* HnsWalletServiceFactory::GetBrowserStateToUse(
    web::BrowserState* context) const {
  return GetBrowserStateRedirectedInIncognito(context);
}

}  // namespace hns_wallet
