/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/ios/browser/hns_wallet/hns_wallet_ipfs_service_factory.h"

#include "hns/components/hns_wallet/browser/hns_wallet_ipfs_service.h"
#include "hns/components/hns_wallet/browser/pref_names.h"
#include "hns/ios/browser/hns_wallet/json_rpc_service_factory.h"
#include "components/keyed_service/core/keyed_service.h"
#include "components/keyed_service/ios/browser_state_dependency_manager.h"
#include "ios/chrome/browser/shared/model/application_context/application_context.h"
#include "ios/chrome/browser/shared/model/browser_state/browser_state_otr_helper.h"
#include "ios/chrome/browser/shared/model/browser_state/chrome_browser_state.h"
#include "ios/web/public/browser_state.h"

namespace hns_wallet {

// static
mojo::PendingRemote<mojom::IpfsService>
HnsWalletIpfsServiceFactory::GetForBrowserState(
    ChromeBrowserState* browser_state) {
  return static_cast<HnsWalletIpfsService*>(
             GetInstance()->GetServiceForBrowserState(browser_state, true))
      ->MakeRemote();
}

// static
HnsWalletIpfsService* HnsWalletIpfsServiceFactory::GetServiceForState(
    ChromeBrowserState* browser_state) {
  return static_cast<HnsWalletIpfsService*>(
      GetInstance()->GetServiceForBrowserState(browser_state, true));
}

// static
HnsWalletIpfsServiceFactory* HnsWalletIpfsServiceFactory::GetInstance() {
  static base::NoDestructor<HnsWalletIpfsServiceFactory> instance;
  return instance.get();
}

HnsWalletIpfsServiceFactory::HnsWalletIpfsServiceFactory()
    : BrowserStateKeyedServiceFactory(
          "HnsWalletIpfsService",
          BrowserStateDependencyManager::GetInstance()) {}

HnsWalletIpfsServiceFactory::~HnsWalletIpfsServiceFactory() = default;

std::unique_ptr<KeyedService>
HnsWalletIpfsServiceFactory::BuildServiceInstanceFor(
    web::BrowserState* context) const {
  auto* browser_state = ChromeBrowserState::FromBrowserState(context);
  std::unique_ptr<HnsWalletIpfsService> ipfs_service(
      new HnsWalletIpfsService(browser_state->GetPrefs()));
  return ipfs_service;
}

bool HnsWalletIpfsServiceFactory::ServiceIsNULLWhileTesting() const {
  return true;
}

web::BrowserState* HnsWalletIpfsServiceFactory::GetBrowserStateToUse(
    web::BrowserState* context) const {
  return GetBrowserStateRedirectedInIncognito(context);
}

}  // namespace hns_wallet
