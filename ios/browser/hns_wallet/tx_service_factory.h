/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_IOS_BROWSER_HNS_WALLET_TX_SERVICE_FACTORY_H_
#define HNS_IOS_BROWSER_HNS_WALLET_TX_SERVICE_FACTORY_H_

#include <memory>

#include "hns/components/hns_wallet/browser/tx_service.h"
#include "hns/components/hns_wallet/common/hns_wallet.mojom.h"
#include "components/keyed_service/ios/browser_state_keyed_service_factory.h"
#include "mojo/public/cpp/bindings/pending_remote.h"

class ChromeBrowserState;
class KeyedService;

namespace base {
template <typename T>
class NoDestructor;
}  // namespace base

namespace web {
class BrowserState;
}  // namespace web

namespace hns_wallet {

class TxServiceFactory : public BrowserStateKeyedServiceFactory {
 public:
  // Creates the service if it doesn't exist already for |browser_state|.
  static mojo::PendingRemote<mojom::TxService> GetForBrowserState(
      ChromeBrowserState* browser_state);
  static mojo::PendingRemote<mojom::EthTxManagerProxy>
  GetEthTxManagerProxyForBrowserState(ChromeBrowserState* browser_state);
  static mojo::PendingRemote<mojom::SolanaTxManagerProxy>
  GetSolanaTxManagerProxyForBrowserState(ChromeBrowserState* browser_state);
  static TxService* GetServiceForState(ChromeBrowserState* browser_state);

  static TxServiceFactory* GetInstance();

 private:
  friend base::NoDestructor<TxServiceFactory>;

  TxServiceFactory();
  ~TxServiceFactory() override;

  // BrowserContextKeyedServiceFactory:
  // BrowserStateKeyedServiceFactory implementation.
  std::unique_ptr<KeyedService> BuildServiceInstanceFor(
      web::BrowserState* context) const override;
  bool ServiceIsNULLWhileTesting() const override;
  web::BrowserState* GetBrowserStateToUse(
      web::BrowserState* context) const override;

  TxServiceFactory(const TxServiceFactory&) = delete;
  TxServiceFactory& operator=(const TxServiceFactory&) = delete;
};

}  // namespace hns_wallet

#endif  // HNS_IOS_BROWSER_HNS_WALLET_TX_SERVICE_FACTORY_H_
