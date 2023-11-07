/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/ios/browser/hns_wallet/tx_service_factory.h"

#include <utility>

#include "base/no_destructor.h"
#include "hns/components/hns_wallet/browser/pref_names.h"
#include "hns/ios/browser/hns_wallet/asset_ratio_service_factory.h"
#include "hns/ios/browser/hns_wallet/json_rpc_service_factory.h"
#include "hns/ios/browser/hns_wallet/keyring_service_factory.h"
#include "components/keyed_service/core/keyed_service.h"
#include "components/keyed_service/ios/browser_state_dependency_manager.h"
#include "ios/chrome/browser/shared/model/browser_state/browser_state_otr_helper.h"
#include "ios/chrome/browser/shared/model/browser_state/chrome_browser_state.h"
#include "ios/web/public/thread/web_task_traits.h"
#include "ios/web/public/thread/web_thread.h"

namespace hns_wallet {

// static
mojo::PendingRemote<mojom::TxService> TxServiceFactory::GetForBrowserState(
    ChromeBrowserState* browser_state) {
  return static_cast<TxService*>(
             GetInstance()->GetServiceForBrowserState(browser_state, true))
      ->MakeRemote();
}

// static
mojo::PendingRemote<mojom::EthTxManagerProxy>
TxServiceFactory::GetEthTxManagerProxyForBrowserState(
    ChromeBrowserState* browser_state) {
  return static_cast<TxService*>(
             GetInstance()->GetServiceForBrowserState(browser_state, true))
      ->MakeEthTxManagerProxyRemote();
}

// static
mojo::PendingRemote<mojom::SolanaTxManagerProxy>
TxServiceFactory::GetSolanaTxManagerProxyForBrowserState(
    ChromeBrowserState* browser_state) {
  return static_cast<TxService*>(
             GetInstance()->GetServiceForBrowserState(browser_state, true))
      ->MakeSolanaTxManagerProxyRemote();
}

// static
TxService* TxServiceFactory::GetServiceForState(
    ChromeBrowserState* browser_state) {
  return static_cast<TxService*>(
      GetInstance()->GetServiceForBrowserState(browser_state, true));
}

// static
TxServiceFactory* TxServiceFactory::GetInstance() {
  static base::NoDestructor<TxServiceFactory> instance;
  return instance.get();
}

TxServiceFactory::TxServiceFactory()
    : BrowserStateKeyedServiceFactory(
          "TxService",
          BrowserStateDependencyManager::GetInstance()) {
  DependsOn(JsonRpcServiceFactory::GetInstance());
  DependsOn(KeyringServiceFactory::GetInstance());
  DependsOn(AssetRatioServiceFactory::GetInstance());
}

TxServiceFactory::~TxServiceFactory() = default;

std::unique_ptr<KeyedService> TxServiceFactory::BuildServiceInstanceFor(
    web::BrowserState* context) const {
  auto* browser_state = ChromeBrowserState::FromBrowserState(context);
  auto* json_rpc_service =
      JsonRpcServiceFactory::GetServiceForState(browser_state);
  auto* keyring_service =
      KeyringServiceFactory::GetServiceForState(browser_state);
  // TODO(apaymyshev): support bitcoin for ios.
  std::unique_ptr<TxService> tx_service(new TxService(
      json_rpc_service, /*bitcoin_wallet_service=*/nullptr, keyring_service,
      browser_state->GetPrefs(), browser_state->GetStatePath(),
      web::GetUIThreadTaskRunner({})));
  return tx_service;
}

bool TxServiceFactory::ServiceIsNULLWhileTesting() const {
  return true;
}

web::BrowserState* TxServiceFactory::GetBrowserStateToUse(
    web::BrowserState* context) const {
  return GetBrowserStateRedirectedInIncognito(context);
}

}  // namespace hns_wallet
