/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_IOS_BROWSER_HNS_WALLET_HNS_WALLET_IPFS_SERVICE_FACTORY_H_
#define HNS_IOS_BROWSER_HNS_WALLET_HNS_WALLET_IPFS_SERVICE_FACTORY_H_

#include <memory>

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

class HnsWalletIpfsService;

class HnsWalletIpfsServiceFactory : public BrowserStateKeyedServiceFactory {
 public:
  HnsWalletIpfsServiceFactory(const HnsWalletIpfsServiceFactory&) = delete;
  HnsWalletIpfsServiceFactory& operator=(
      const HnsWalletIpfsServiceFactory&) = delete;

  // Creates the service if it doesn't exist already for |browser_state|.
  static mojo::PendingRemote<mojom::IpfsService> GetForBrowserState(
      ChromeBrowserState* browser_state);
  static HnsWalletIpfsService* GetServiceForState(
      ChromeBrowserState* browser_state);

  static HnsWalletIpfsServiceFactory* GetInstance();

 private:
  friend base::NoDestructor<HnsWalletIpfsServiceFactory>;

  HnsWalletIpfsServiceFactory();
  ~HnsWalletIpfsServiceFactory() override;

  // BrowserContextKeyedServiceFactory:
  // BrowserStateKeyedServiceFactory implementation.
  std::unique_ptr<KeyedService> BuildServiceInstanceFor(
      web::BrowserState* context) const override;
  bool ServiceIsNULLWhileTesting() const override;
  web::BrowserState* GetBrowserStateToUse(
      web::BrowserState* context) const override;
};

}  // namespace hns_wallet

#endif  // HNS_IOS_BROWSER_HNS_WALLET_HNS_WALLET_IPFS_SERVICE_FACTORY_H_
